# -*- coding: UTF-8 -*-
import kungfu.yijinjing.time as kft
from pykungfu import wingchun as wc
import yaml
import pandas as pd
import numpy as np
import sys, os
from datetime import datetime
basedir = "/home/wangzirui/workspace/FlowDataTradeSystem"
sys.path.insert(0, basedir)
from kungfu.wingchun.constants import *
import dolphindb as ddb
import yaml

from FlowDataTradeSystem.feature.feature_builder import FeatureBuilder
from FlowDataTradeSystem.feature.future_feature_builder import FutureFeatureBuilder
from FlowDataTradeSystem.factor.factor_builder import FactorBuilder
from FlowDataTradeSystem.factor.factor_loader import load_factors_from_directory
from FlowDataTradeSystem.model.model_base import ModelBase
from FlowDataTradeSystem.tests.utils.prepareData import (
    prepareTrade, prepareOrder, prepareQuote, prepareThreshold, prepareFutureQuote
)
from FlowDataTradeSystem.tests.utils.extractInfo import (
    afterMarketInformation
)
from FlowDataTradeSystem.tests.utils.storage import (
    save_object, load_object
)
from FlowDataTradeSystem.tests.utils.config import *
# from FlowDataTradeSystem.tests.utils.rqutil import *
# from FlowDataTradeSystem.factor.factor_builder import myprint

with open(f"{basedir}/FlowDataTradeSystem/tests/config/strategyv3_config.yml", "r", encoding='utf-8') as f:
    config = yaml.safe_load(f)

# 股票柜台
SOURCE = config['SOURCE']
# 行情源
SOURCE_md = config['SOURCE_md']
SOURCE_future_md = config['SOURCE_future_md']
# 要链接的账户
ACCOUNT = config['ACCOUNT']
# 准备订阅的标的
tickers_SH = config['tickers_SH']
# tickers_SZ = ["159673", "159919"]
tickers_SZ = config['tickers_SZ']
tickers = tickers_SH + tickers_SZ
# tickers_Future = [get_dominant_future("IF", datetime.now().date())]
tickers_Future = config['tickers_Future']
tickers_pair = {ticker: tickers_Future[0] for ticker in tickers_SH + tickers_SZ}
# tickers = ['510310']
# 下单数量
VOLUME = 10000
# 标的对应的交易所
EXCHANGE = config['EXCHANGE']
EXCHANGE_SH = config['EXCHANGE_SH']
EXCHANGE_SZ = config['EXCHANGE_SZ']
EXCHANGE_FUTURE = config['EXCHANGE_FUTURE']

# 启动后多久不交易
WAITING_MIN = config['WAITING_MIN']

# 动态加载所有因子模块
factors_dir = f"{basedir}/FlowDataTradeSystem/factor/factors"
# 加载factor/factors下的因子
load_factors_from_directory(os.path.join(factors_dir, ""), "factor/factors")
factors_dir = f"{basedir}/FlowDataTradeSystem/factor/future_factors"
load_factors_from_directory(os.path.join(factors_dir, ""), "factor/future_factors")

# featureBuilder = FeatureBuilder()
# config_filepath = rf'{basedir}\FlowDataTradeSystem\factor\factors_config.yml'
# factorBuilder = FactorBuilder(featureBuilder, config_filepath)

s = ddb.session()
s.enableStreaming()


factor_study_basedir = '/public/CalculateFactor_ByDDBAPI/fund'
def get_model_dict():
    # model_dict = ModelBase.create_model("lin_model", "")
    model_dict = {}
    for ticker in tickers:
        filepath = f'{factor_study_basedir}/factor_eval_summary/param_optimized/model/{ticker}'
    
    folder_path = f"{basedir}/FlowDataTradeSystem/tests/data"
    for filename in os.listdir(folder_path):
        if os.path.isfile(os.path.join(folder_path, filename)) and 'lin_model_allfac_test_withfuture' in filename:
            symbol = filename[-9:]
            model = ModelBase.create_model("lin_model", os.path.join(folder_path, filename), None)
            model_dict[symbol] = model
    return model_dict

config_filepath = f'{basedir}/FlowDataTradeSystem/factor/factors_config.yml'
future_config_filepath = f'{basedir}/FlowDataTradeSystem/factor/future_factors_config.yml'



model_dict = get_model_dict()
threshold_info = prepareThreshold(folder_path=f"{factor_study_basedir}/factor_comb_test_with_future_factor/threshold_info")

par_dict = {
    'preprocess_filepath': factor_study_basedir + "/factor_comb_test_with_future_factor/data_process_params/{}.yml",
    'model_dict': model_dict,
    'judge_col': 'spread1_fund',
    'no_winsorize_factors': ['DeriPBid1_future', 'DeriPBid1_fund'],
    'buy_threshold': threshold_info['buy_threshold'],
    'sell_threshold': threshold_info['sell_threshold'],
    'close_buy_threshold': threshold_info['close_buy_threshold'],
    'close_sell_threshold': threshold_info['close_sell_threshold'],
}
# 特征类对象列表(其中原始数据)的备份路径
STORAGE_FILEPATH = STORAGE_DIRPATH + f"/feat_{datetime.now().date()}.pkl"
POS_STRORAGE_FILEPATH = STORAGE_DIRPATH + f"/pos_{datetime.now().date()}.pkl"



# 启动前回调，添加交易账户，订阅行情，策略初始化计算等
def pre_start(context):
    context.add_account(SOURCE, ACCOUNT)
    context.subscribe(SOURCE_md, tickers_SH, EXCHANGE_SH)
    # context.subscribe(SOURCE_md, tickers_SZ, EXCHANGE_SZ)
    context.subscribe(SOURCE_future_md, tickers_Future, EXCHANGE_FUTURE)
    context.depth_orderbook = {}
    context.log.info("pre_start")
    featBuilderDict = load_object(context, STORAGE_FILEPATH)
    positionDict = load_object(context, POS_STRORAGE_FILEPATH)
    if featBuilderDict is None:
        featBuilderDict = {}
        for ticker in tickers:
            featBuilderDict[ticker] = FeatureBuilder()
        for ticker in tickers_Future:
            featBuilderDict[ticker] = FutureFeatureBuilder()
    else:
        for ticker in tickers:
            if ticker not in featBuilderDict.keys():
                featBuilderDict[ticker] = FeatureBuilder()
        for ticker in tickers_Future:
            if ticker not in featBuilderDict.keys():
                featBuilderDict[ticker] = FutureFeatureBuilder()
                
    if positionDict is None:
        positionDict = {}
        for ticker in tickers:
            positionDict[ticker] = {"long": 0, "short": 0}
    else:
        for ticker in tickers:
            if ticker not in positionDict.keys():
                positionDict[ticker] = {"long": 0, "short": 0}

    facBuilderDict = {}
    for ticker in tickers:
        facBuilderDict[ticker] = FactorBuilder(featBuilderDict[ticker], config_filepath)
    for ticker in tickers_Future:
        facBuilderDict[ticker] = FactorBuilder(featBuilderDict[ticker], future_config_filepath)
        
    # context.log.warning("{}".format(list(featBuilderDict.keys())))
    # context.log.warning("{}".format(list(facBuilderDict.keys())))
    
    context.featBuilderDict = featBuilderDict
    context.facBuilderDict = facBuilderDict
    context.positionDict = positionDict
    context.faccomb_time = None
    # context.log.info(featBuilderDict.keys())
    
    context.start_time = context.now()


# 启动准备工作完成后回调，策略只能在本函数回调以后才能进行获取持仓和报单
def post_start(context):
    context.log.warning("post_start")
    log_book(context, None)


# 收到快照行情时回调，行情信息通过quote对象获取
def on_quote(context, quote, location,dest):
    context.log.info("[on_quote] {}".format(quote))
    
    
    time = pd.to_datetime(context.strftime(context.now()))
    if (time.time() < pd.to_datetime("09:30:00").time()) or\
        (time.time() > pd.to_datetime("15:00:00").time() or\
        (time.time() > pd.to_datetime("11:30:00").time() and time.time() < pd.to_datetime("13:00:00").time())):
        return
    
    if (time.time() > pd.to_datetime("14:56:00").time()) and ( quote.exchange_id == EXCHANGE_SZ or quote.exchange_id == EXCHANGE_SH):
        close_today_position(context, quote)
        return
    
    featureBuilder = context.featBuilderDict[quote.instrument_id]
    factorBuilder = context.facBuilderDict[quote.instrument_id]
    
        
    if quote.instrument_id in tickers_Future:
        data = prepareFutureQuote(quote)
        # context.log.info(data)
        featureBuilder.build_snap_features(data)
        factorBuilder.compute_all_factors()
    elif quote.exchange_id == EXCHANGE_SZ or quote.exchange_id == EXCHANGE_SH:
        data = prepareQuote(quote)
        # 更新最新的盘口价格
        context.depth_orderbook[quote.instrument_id] = {'bid1': data['bid_prices'][0], 'ask1': data['ask_prices'][0]}
        featureBuilder.build_snap_features(data)
    
    
def construct_factor_and_sendOrder(context, entrust):
    for ticker in tickers:
        featureBuilder = context.featBuilderDict[ticker]
        featureBuilder.builder_other_features(context.faccomb_time)
        
        factorBuilder = context.facBuilderDict[ticker]
        fund_results = factorBuilder.comput_all_factors()
        
        future_ticker = tickers_Future[0]
        future_results = context.facBuilderDict[future_ticker].get_results()
        
        results = future_results.copy()
        for key, value in fund_results.items():
            results[key+'_fund'] = value

        if ticker in tickers_SH:
            symbol_ = ticker + ".SH"
        elif ticker in tickers_SZ:
            symbol_ = ticker + ".SZ"
        preprocess_filepath = par_dict['preprocess_filepath'].format(symbol_)
        factors = factorBuilder.preprocess(preprocess_filepath, par_dict['judge_col'],
                                                    par_dict['no_winsorize_factors'],
                                                    factors=results.copy())
        
        if factors is None:
            return
        
        model = par_dict['model_dict'][symbol_]
        model_v = model.predict(factors)
        signal = generate_signal(par_dict, symbol_, model_v)
        close_signal = generate_close_signal(par_dict, symbol_, model_v)
        order_id = 0
        
        
        # 程序启动不到1分钟则不进行下单
        if context.now() - context.start_time <= 1000*60*WAITING_MIN:
            return
        # context.log.warning("{}".format(context.depth_orderbook))
        bid1 = context.depth_orderbook[ticker]['bid1'] 
        ask1 = context.depth_orderbook[ticker]['ask1'] 
            
        context.log.info("[threshold]{} [factor comb] (symbol){} (value){} (signal){} (close_signal){}".format(par_dict['buy_threshold'][symbol_], ticker, model_v, signal, close_signal))
        if (close_signal == 1) and can_place_order(context, ticker, VOLUME, None, Side.Sell, Offset.Close):
            # pass
            vol = can_place_order(context, ticker, VOLUME, ask1, Side.Sell, Offset.Close)
            
            order_id = context.insert_order(ticker, EXCHANGE, SOURCE, ACCOUNT, ask1,
                                            vol,
                                            PriceType.Limit, Side.Sell, Offset.Close)
        elif (close_signal == -1) and can_place_order(context, ticker, VOLUME, bid1, Side.Buy, Offset.Close):
            # pass
            order_id = context.insert_order(ticker, EXCHANGE, SOURCE, ACCOUNT, bid1,
                                            VOLUME,
                                            PriceType.Limit, Side.Buy, Offset.Close)
        elif (signal == 1) and can_place_order(context, ticker, VOLUME, bid1, Side.Buy, Offset.Open):
            # pass
            order_id = context.insert_order(ticker, EXCHANGE, SOURCE, ACCOUNT, bid1,
                                            VOLUME,
                                            PriceType.Limit, Side.Buy, Offset.Open)
            if order_id > 0:
                # 通过添加时间回调，在三秒以后撤单
                context.add_timer(context.now() + 3 * 1000000000, lambda ctx, event: cancel_order(ctx, order_id))
        elif (signal == -1) and can_place_order(context, ticker, VOLUME, ask1, Side.Sell, Offset.Open):
            # pass
            vol = can_place_order(context, ticker, VOLUME, ask1, Side.Sell, Offset.Open)
            order_id = context.insert_order(ticker, EXCHANGE, SOURCE, ACCOUNT, ask1,
                                            vol,
                                            PriceType.Limit, Side.Sell, Offset.Open)
            if order_id > 0:
                # 通过添加时间回调，在三秒以后撤单
                context.add_timer(context.now() + 3 * 1000000000, lambda ctx, event: cancel_order(ctx, order_id))
        


# 收到订单状态回报时回调
def on_order(context, order, location, dest):
    context.log.info("[on_order] {}".format(order))
    # if (order.order_id > 0 and order.offset == Offset.Open):
    #     # 通过添加时间回调，在三秒以后撤单
    #     context.add_timer(context.now() + 3 * 1000000000, lambda ctx, event: cancel_order(ctx, order))


# 收到成交信息回报时回调
def on_trade(context, trade, location, dest):
    context.log.info("[on_trade] {}".format(trade))
    instrument_id = trade.instrument_id
    if trade.offset == Offset.Open:
        if trade.side == Side.Buy :
            context.positionDict[instrument_id]['long'] += trade.volume
        elif trade.side == Side.Sell:
            context.positionDict[instrument_id]['short'] += trade.volume
    elif trade.offset == Offset.Close:
        if trade.side == Side.Buy :
            context.positionDict[instrument_id]['short'] -= trade.volume
        elif trade.side == Side.Sell:
            context.positionDict[instrument_id]['long'] -= trade.volume
    context.log.info("[instrument_id] {} [long] {} [short] {}".format(instrument_id,   context.positionDict[instrument_id]['long'], context.positionDict[instrument_id]['short']))



def on_transaction(context, transaction, location, dest):
    # if transaction.exec_type == ExecType.Cancel:
    #     context.log.info('[on_transaction] {}'.format(transaction))
    
    featureBuilder = context.featBuilderDict[transaction.instrument_id]
    trade = prepareTrade(transaction)
    featureBuilder.add_transaction(trade)

def on_entrust(context, entrust, location, dest):
    # context.log.info('[on_entrust] {}'.format(entrust))
    featureBuilder = context.featBuilderDict[entrust.instrument_id]
    order = prepareOrder(entrust)
    if order['datetime'].time() < pd.to_datetime('09:30:00').time():
        featureBuilder.entrust_dict_by_appl_seq[order['orig_order_no']] = order
        return
    
    # 更新需要进行因子组合的时刻，有逐笔委托信息的时间进行触发因子组合，触发后完成所有标的的因子组合
    if context.faccomb_time is None:
        context.faccomb_time = get_aligned_time(entrust['datetime'])
    elif entrust['datetime'] > context.faccomb_time:
            construct_factor_and_sendOrder(context, entrust)
            context.faccomb_time = context.faccomb_time + pd.Timedelta(seconds=3)
                
    featureBuilder.add_entrust(order)

def get_aligned_time(given_time, base_time="09:30:00"):
    base_time = pd.Timestamp(base_time)

    elapsed_time = given_time - base_time
    elapsed_ms = (given_time - base_time).total_seconds() * 1000

    interval_ms = 3000
    a = 1 if (elapsed_ms % interval_ms) > 0 else 0
    aligned_ms = (elapsed_ms // interval_ms + a) * interval_ms  # 向上取整
    aligned_time = base_time + pd.to_timedelta(aligned_ms, unit="ms")
    return aligned_time

# 策略退出前方法，仍然可以获取持仓和报单
def pre_stop(context):
    afterMarketInformation(context, dirpath = f"{basedir}/FlowDataTradeSystem/tests/account_data")
    context.log.info("[before strategy stop]")


# 策略进程退出前方法
def post_stop(context):
    save_object(context, context.featBuilderDict, STORAGE_FILEPATH)
    save_object(context, context.positionDict, POS_STRORAGE_FILEPATH)
    context.log.info("[before process stop]")


def close_today_position(context, quote):
    long_pos = context.positionDict[quote.instrument_id]["long"]
    short_pos = context.positionDict[quote.instrument_id]["short"]
    
    if (long_pos > 0):
        # price = context.depth_orderbook[quote.instrument_id]['bid1']
        price = quote.bid_price[0]
        order_id = context.insert_order(quote.instrument_id, EXCHANGE, SOURCE, ACCOUNT, price, long_pos,
                                            PriceType.Limit, Side.Sell, Offset.Close)
        if order_id > 0:
                # 通过添加时间回调，在15秒以后撤单
                context.add_timer(context.now() + 15 * 1000000000, lambda ctx, event: cancel_reorder(ctx, order_id))
        context.log.info("[close today long pos] (rid){} (ticker){} (rem_pos){}".format(order_id, quote.instrument_id, long_pos))
    elif (short_pos > 0):
        # price = context.depth_orderbook[quote.instrument_id]['ask1']
        price = quote.ask_price[0]
        order_id = context.insert_order(quote.instrument_id, EXCHANGE, SOURCE, ACCOUNT, price, short_pos,
                                        PriceType.Limit, Side.Buy, Offset.Close)
        if order_id > 0:
                # 通过添加时间回调，在15秒以后撤单
                context.add_timer(context.now() + 15 * 1000000000, lambda ctx, event: cancel_reorder(ctx, order_id))
        context.log.info("[close today short pos] (rid){} (ticker){} (rem_pos){}".format(order_id, quote.instrument_id, short_pos))
        
    

def can_place_order(context, instrument_id, volume, price, side, offset):
    if offset == Offset.Open:
        if side == Side.Buy:
            flag = has_sufficient_balance(context, instrument_id, volume, price, side)
            return flag
        elif side == Side.Sell:
            flag = has_sufficient_position(context, instrument_id, volume, side)
            return flag
    elif offset == Offset.Close:
        if side == Side.Buy:
            min_vol = min(context.positionDict[instrument_id]['short'], volume)
            if min_vol <= 0:
                return False
            flag = has_sufficient_balance(context, instrument_id, min_vol, price, side)
            return flag
        elif side == Side.Sell:
            min_vol = min(context.positionDict[instrument_id]['long'], volume)
            if min_vol <= 0:
                return 0
            flag = has_sufficient_position(context, instrument_id, min_vol, side)
            return flag


# 判断是否有充足的仓位
def has_sufficient_position(context, instrument_id, volume, side):
    account_uid = context.get_account_uid(SOURCE, ACCOUNT)
    position_key = None
    if instrument_id in tickers_SH:
        position_key = wc.utils.hash_instrument(account_uid, EXCHANGE_SH, instrument_id)
    elif instrument_id in tickers_SZ:
        position_key = wc.utils.hash_instrument(account_uid, EXCHANGE_SZ, instrument_id)
    position_volume = 0
    
    try:
        position = context.get_account_book(SOURCE, ACCOUNT).long_positions[position_key]
        # context.log.info("{} {}".format(position.instrument_id, position.yesterday_volume))
        position_volume = position.yesterday_volume
    except KeyError as e:
        context.log.warning("???")
        return False
    # if side == Side.Sell:
    #     try:
    #         position = context.get_account_book(SOURCE, ACCOUNT).long_positions[position_key]
    #         # context.log.info("{} {}".format(position.instrument_id, position.yesterday_volume))
    #         position_volume = position.yesterday_volume
    #     except KeyError as e:
    #         context.log.warning("???")
    #         return False
    # elif side == Side.Buy:
    #     try:
    #         position = context.get_account_book(SOURCE, ACCOUNT).short_positions[position_key]
    #         position_volume = position.yesterday_volume
    #     except KeyError as e:
    #         return False
    if position_volume > 0:
        return min(position_volume, volume)
    else:
        return 0


# 判断是否有充足的资金
def has_sufficient_balance(context, symbol, volume, price, side):
    # print(volume, price)
    need_amount = volume * price
    book = context.get_account_book(SOURCE, ACCOUNT)
    min_commission = 0.001
    context.log.warning("可用资金{} 所需资金{} ".format(book.asset.avail, need_amount * (1 + min_commission)))
    if need_amount * (1 + min_commission) < 0:
        context.log.warning("所需资金小于0 [volume]{} [price]{} [min_commission]{}".format(volume, price, min_commission))
    if book.asset.avail > need_amount * (1 + min_commission):
        return True
    else:
        return False


# 自定义函数
# 账户中资金/持仓情况
def log_book(context, event):
    context.account_book = context.get_account_book(SOURCE, ACCOUNT)

    context.log.warning("账户资金组合信息 {}".format(context.account_book.asset))
     
    # 账户中多头持仓数据
    long_position = context.account_book.long_positions
    for key in long_position:
        pos = long_position[key]
        
        account_uid = context.get_account_uid(SOURCE, ACCOUNT)
        position_key = None
        if pos.instrument_id in tickers_SH:
            position_key = wc.utils.hash_instrument(account_uid, EXCHANGE_SH, pos.instrument_id)
        elif pos.instrument_id in tickers_SZ:
            position_key = wc.utils.hash_instrument(account_uid, EXCHANGE_SZ, pos.instrument_id)
        
        context.log.info("多头持仓数据 (instrument_id){} (volume){} (yesterday_volume){} key{} position_key{}".format(pos.instrument_id,pos.volume,pos.yesterday_volume, key, position_key))


# 自定义撤单回调函数
def cancel_order(context, order_id):
    action_id = context.cancel_order(order_id)
    if action_id > 0:
        context.log.info("[cancel order] (action_id){} (rid){} ".format(action_id, order_id))


# 自定义撤单并对未成交的进行补充下单的回调函数
def cancel_reorder(context, order_id):
    order = context.book.orders[order_id]
    action_id = context.cancel_order(order_id)
    if action_id > 0:
        context.log.info("[cancel and reorder] (action_id){} (rid){} ".format(action_id, order_id))
    if order.side == Side.Buy:
        price = context.depth_orderbook[order.instrument_id]['bid1']
        order_id = context.insert_order(order.instrument_id, EXCHANGE, SOURCE, ACCOUNT, price, order.volume_left,
                                        PriceType.Limit, Side.Buy, Offset.Open)
    elif order.side == Side.Sell:
        price = context.depth_orderbook[order.instrument_id]['ask1']
        order_id = context.insert_order(order.instrument_id, EXCHANGE, SOURCE, ACCOUNT, price, order.volume_left,
                                        PriceType.Limit, Side.Sell, Offset.Open)


def generate_signal(context, symbol, model_v):
    # 1代表买入 -1代表卖出
    if model_v > context['buy_threshold'][symbol]:
        return 1
    elif model_v < context['sell_threshold'][symbol]:
        return -1
    else:
        return 0

def generate_close_signal(context, symbol, model_v):
    if model_v < context['close_buy_threshold'][symbol]:
        return 1
    elif model_v > context['close_sell_threshold'][symbol]:
        return -1
    else:
        return 0