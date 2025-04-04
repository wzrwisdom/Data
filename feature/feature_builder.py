from sortedcontainers import SortedDict
from collections import deque
import math
from loguru import logger as log
import numpy as np
import pandas as pd
from datetime import datetime

def get_aligned_time(given_time, base_time="09:30:00"):
    base_time = pd.Timestamp(base_time)

    elapsed_time = given_time - base_time
    elapsed_ms = (given_time - base_time).total_seconds() * 1000

    interval_ms = 3000
    a = 1 if (elapsed_ms % interval_ms) > 0 else 0
    aligned_ms = (elapsed_ms // interval_ms + a) * interval_ms  # 向上取整
    aligned_time = base_time + pd.to_timedelta(aligned_ms, unit="ms")
    return aligned_time

class FeatureBuilder:
    def __init__(self):
        # 生成交易时间内的时间索引
        start_time = pd.Timestamp('09:30:03')
        end_time = pd.Timestamp('11:30:00')
        self.time_index = pd.date_range(start=start_time, end=end_time, freq='3s')
        start_time = pd.Timestamp('13:00:00')
        end_time = pd.Timestamp('14:57:00')
        self.time_index = self.time_index.append(pd.date_range(start=start_time, end=end_time, freq='3s'))
        # 生成存储特征的数组
        self.initialize_feat_matrix()

        self.start_index = None
        self.cur_time = None
        self.trade_buffer = deque()
        self.entrust_buffer = deque()
        self.cancel_buffer = deque()
        self.entrust_dict_by_appl_seq = {}

    def initialize_feat_matrix(self):
        feat_cols = [
            's1', 'b1', 's5', 'b5', 's10', 'b10',
            'sv1_sum', 'bv1_sum', 'ssv1_sum', 'bbv1_sum', 'sv5_sum', 'bv5_sum', 'bbv5_sum', 'ssv5_sum', 'sv10_sum', 'bv10_sum', 'ssv10_sum', 'bbv10_sum',
            'wb1', 'wb5', 'wb10', 'bs_avg_price'
        ]
        feat_cols += [
            'open', 'close', 'high', 'low', 'vwap',
            'td_buy_num', 'td_sell_num', 'td_buy_price', 'td_sell_price', 'td_buy_vol', 'td_sell_vol', 'td_vol', 'td_price_std'
        ]
        feat_cols += [
            'en_buy_num', 'en_sell_num', 'en_buy_price', 'en_sell_price', 'en_buy_vol', 'en_sell_vol', 'en_price_std'
        ]
        feat_cols += [
            'cancel_buy_num', 'cancel_sell_num', 'cancel_buy_vol', 'cancel_sell_vol',
            'cancel_buy_time_range', 'cancel_sell_time_range', 'cancel_buy_time_med', 'cancel_sell_time_med'
        ]
        self.feat = pd.DataFrame(
            np.full((len(self.time_index), len(feat_cols)), np.nan),
            index=self.time_index, columns=feat_cols
        )

        self.ffill_feats = [
            's1', 'b1', 's5', 'b5', 's10', 'b10',
            'sv1_sum', 'bv1_sum', 'ssv1_sum', 'bbv1_sum', 'sv5_sum', 'bv5_sum', 'bbv5_sum', 'ssv5_sum', 'sv10_sum', 'bv10_sum', 'ssv10_sum', 'bbv10_sum',
            'wb1', 'wb5', 'wb10', 'bs_avg_price',
            'close', 'vwap',
        ]
        self.fillzero_feats = [
            'td_buy_num', 'td_sell_num', 'td_buy_vol', 'td_sell_vol', 'td_vol',
            'en_buy_num', 'en_sell_num', 'en_buy_vol', 'en_sell_vol',
            'cancel_buy_num', 'cancel_sell_num', 'cancel_buy_vol', 'cancel_sell_vol',
        ]

    def build_feature_extractor(self, cur_time):
        end_index = self.time_index.get_indexer([get_aligned_time(cur_time)])[0] + 1
        def get_recent_features(name, window):
            if name not in self.feat.columns:
                log.error(f"特征类中没有{name}的数据")
            if window is None:
                start_index = 0
            else:
                start_index = end_index - window if end_index - window >= 0 else 0
            if self.start_index is not None:
                start_index = max(start_index, self.start_index)
            else:
                log.warning("self.start_index is None")
                start_index = end_index - 1
            return self.feat.iloc[start_index:end_index][name].reset_index(drop=True)
        return get_recent_features

    def add_feature(self, index, feature_info):
        cur_time = self.time_index[index]
        for name, value in feature_info.items():
            last_valid_time = self.feat[name].loc[:cur_time].last_valid_index()
            last_valid_index = self.time_index.get_indexer([last_valid_time])[0]
            # 判断特征是否存在空缺，将空缺部分进行填充
            if last_valid_time and (last_valid_index + 1 < index):
                fill_range = self.feat.loc[last_valid_time:cur_time, name]

                if name in self.ffill_feats:
                    self.feat.loc[last_valid_time:cur_time, name] = fill_range.ffill()
                elif name in self.fillzero_feats:
                    self.feat.loc[last_valid_time:cur_time, name] = fill_range.fillna(0)
                elif name in ['open', 'high', 'low']:
                    # self.feat.iloc[last_valid_index+1:index].loc[name] = self.feat.loc[last_valid_time, 'close']
                    self.feat.loc[self.feat.index[last_valid_index+1:index], name] = self.feat.loc[last_valid_time, 'close']

            self.feat.loc[cur_time, name] = value


    def build_snap_features(self, data):
        ##########   构建特征所需函数   ######
        def calculate_wb(bv_sum, sv_sum, last_wb):
            try:
                return (bv_sum - sv_sum) / (bv_sum + sv_sum)
            except ZeroDivisionError:
                return last_wb
        ###################################
        features = {}
        cur_time = pd.Timestamp(datetime.now()).normalize() + pd.to_timedelta(
            data['datetime'].time().strftime("%H:%M:%S.%f"))
        feat_index = self.time_index.get_indexer([get_aligned_time(cur_time)])[0].item()
        if feat_index == -1:
            return None
        if self.start_index is None:
            self.start_index = feat_index    
        
        sv1_sum = data['ask_volumes'][0]
        bv1_sum = data['bid_volumes'][0]
        features.update({
            's1': data['ask_prices'][0], 's5': data['ask_prices'][4], 's10': data['ask_prices'][9],
            'b1': data['bid_prices'][0], 'b5': data['bid_prices'][4], 'b10': data['bid_prices'][9],
            'sv1_sum': sv1_sum, 'bv1_sum': bv1_sum,
        })
        features.update({
            'ssv1_sum': features['s1'] * features['sv1_sum'],
            'bbv1_sum': features['b1'] * features['bv1_sum'],
        })

        sv5_sum, bv5_sum = 0, 0
        ssv5_sum, bbv5_sum = 0, 0
        for i in range(0, 5):
            sv5_sum += data['ask_volumes'][i]
            bv5_sum += data['bid_volumes'][i]
            ssv5_sum += data['ask_prices'][i] * data['ask_volumes'][i]
            bbv5_sum += data['bid_prices'][i] * data['bid_volumes'][i]

        sv10_sum, bv10_sum = sv5_sum, bv5_sum
        ssv10_sum, bbv10_sum = ssv5_sum, bbv5_sum
        for i in range(5, 10):
            sv10_sum += data['ask_volumes'][i]
            bv10_sum += data['bid_volumes'][i]
            ssv10_sum += data['ask_prices'][i] * data['ask_volumes'][i]
            bbv10_sum += data['bid_prices'][i] * data['bid_volumes'][i]

        wb1 = calculate_wb(bv1_sum, sv1_sum, last_wb=features.get('wb1', 0))
        wb5 = calculate_wb(bv5_sum, sv5_sum, last_wb=features.get('wb5', 0))
        wb10 = calculate_wb(bv10_sum, sv10_sum, last_wb=features.get('wb10', 0))
        bs_avg_price = (data['ask_prices'][0] + data['bid_prices'][0]) / 2

        features.update({
            # 'close': data["last_price"],
            'sv5_sum': sv5_sum, 'bv5_sum': bv5_sum,
            'ssv5_sum': ssv5_sum, 'bbv5_sum': bbv5_sum,
            'sv10_sum': sv10_sum, 'bv10_sum': bv10_sum,
            'ssv10_sum': ssv10_sum, 'bbv10_sum': bbv10_sum,
            'wb1': wb1, 'wb5': wb5, 'wb10': wb10, 'bs_avg_price': bs_avg_price
        })
        self.add_feature(feat_index, features)
        return self.time_index[feat_index]
        
    def build_snap_features_byDDB(self, ddb_data, time):
        ##########   构建特征所需函数   ######
        def calculate_wb(bv_sum, sv_sum, last_wb):
            try:
                return (bv_sum - sv_sum) / (bv_sum + sv_sum)
            except ZeroDivisionError:
                return last_wb
        ###################################
        new_second = time.second - time.second%3
        time = time.replace(second=new_second, microsecond=0)
        time = pd.Timestamp(datetime.now()).normalize() + pd.to_timedelta(
            time.time().strftime("%H:%M:%S.%f")
        )
        feat_index = self.time_index.get_indexer([time])[0]
        if np.isnan(self.feat.iloc[feat_index]['s1']):
            sv1_sum = ddb_data['ask1_vol']
            bv1_sum = ddb_data['bid1_vol']
            
            features = {
                "s1": ddb_data['ask1'], "b1": ddb_data['bid1'],
                "s5": ddb_data['ask5'], "b5": ddb_data['bid5'],
                "s10": ddb_data['ask10'], "b10": ddb_data['bid10'],
                'sv1_sum': sv1_sum, 'bv1_sum': bv1_sum,
            }
            
            sv5_sum, bv5_sum = 0, 0
            ssv5_sum, bbv5_sum = 0, 0
            for i in range(1, 5+1):
                sv5_sum += ddb_data[f'ask{i}_vol']
                bv5_sum += ddb_data[f'bid{i}_vol']
                ssv5_sum += ddb_data[f'ask{i}_vol'] * ddb_data[f"ask{i}"]
                bbv5_sum += ddb_data[f'bid{i}_vol'] * ddb_data[f"bid{i}"]

            sv10_sum, bv10_sum = sv5_sum, bv5_sum
            ssv10_sum, bbv10_sum = ssv5_sum, bbv5_sum
            for i in range(6, 10+1):
                sv10_sum += ddb_data[f'ask{i}_vol']
                bv10_sum += ddb_data[f'bid{i}_vol']
                ssv10_sum += ddb_data[f'ask{i}_vol'] * ddb_data[f"ask{i}"]
                bbv10_sum += ddb_data[f'bid{i}_vol'] * ddb_data[f"bid{i}"]
            wb1 = calculate_wb(bv1_sum, sv1_sum, last_wb=features.get('wb1', 0))
            wb5 = calculate_wb(bv5_sum, sv5_sum, last_wb=features.get('wb5', 0))
            wb10 = calculate_wb(bv10_sum, sv10_sum, last_wb=features.get('wb10', 0))
            bs_avg_price = (ddb_data["ask1"] + ddb_data["bid1"]) / 2

            features.update({
                'sv5_sum': sv5_sum, 'bv5_sum': bv5_sum,
                'ssv5_sum': ssv5_sum, 'bbv5_sum': bbv5_sum,
                'sv10_sum': sv10_sum, 'bv10_sum': bv10_sum,
                'ssv10_sum': ssv10_sum, 'bbv10_sum': bbv10_sum,
                'wb1': wb1, 'wb5': wb5, 'wb10': wb10, 'bs_avg_price': bs_avg_price
            })
            # log.info("[build_snap_features_byDDB] {}".format(features))
            self.add_feature(feat_index, features)
            
    
    def fill_snap_features(self, time):
        new_second = time.second - time.second%3
        time = time.replace(second=new_second, microsecond=0)
        time = pd.Timestamp(datetime.now()).normalize() + pd.to_timedelta(
            time.time().strftime("%H:%M:%S.%f")
        )
        feat_index = self.time_index.get_indexer([time])[0]
        if np.isnan(self.feat.iloc[feat_index]['s1']) and (feat_index-1 >= 0) and (not np.isnan(self.feat.iloc[feat_index-1]['s1'])):
            feat_cols = [
                's1', 'b1', 's5', 'b5', 's10', 'b10',
                'sv1_sum', 'bv1_sum', 'ssv1_sum', 'bbv1_sum', 'sv5_sum', 'bv5_sum', 'bbv5_sum', 'ssv5_sum', 'sv10_sum', 'bv10_sum', 'ssv10_sum', 'bbv10_sum',
                'wb1', 'wb5', 'wb10', 'bs_avg_price'
            ]
            for col in feat_cols:
                self.feat.loc[self.feat.index[feat_index], col] = self.feat.iloc[feat_index-1][col]
                
        
    def build_other_features(self, time):
        new_second = time.second - time.second%3
        time = time.replace(second=new_second, microsecond=0)
        time = pd.Timestamp(datetime.now()).normalize() + pd.to_timedelta(
            time.time().strftime("%H:%M:%S.%f")
        )
        feat_index = self.time_index.get_indexer([time])[0]
        # 考虑到快照信息是按照给定的频率发送的，以接收到快照信息的时点为判断，进行计算成交特征以及委托特征
        self.build_transaction_features(feat_index)
        self.build_entrust_features(feat_index)
        self.build_cancel_features(feat_index)




    def add_transaction(self, data):
        # 将新的成交数据存入 trade_buffer 中
        if data['exec_type'] == 'D':
            new_data = {
                'symbol': data['symbol'],
                'datetime': data['datetime'],
                'appl_seq_num': data['ask_appl_seq_num'] if data["bid_appl_seq_num"] == 0 else data['bid_appl_seq_num'],
                'side': 'S' if data["bid_appl_seq_num"] == 0 else 'B',
                'volume': data['trade_volume'],
            }
            self.cancel_buffer.append(new_data)
        else:
            self.trade_buffer.append(data)

    def add_entrust(self, data):
        # 将新的委托数据存入 entrust_buffer 中
        if data['order_type'] == 'D':
            new_data = {
                'symbol': data['symbol'],
                'datetime': data['datetime'],
                'appl_seq_num': data['appl_seq_num'],
                'side': data['side'],
                'volume': data['order_volume'],
            }
            # print(new_data)
            self.cancel_buffer.append(new_data)
        else:
            self.entrust_dict_by_appl_seq[data['orig_order_no']] = data
            self.entrust_buffer.append(data)

    def build_transaction_features(self, feat_index):
        ##########   构建特征所需函数   ######
        def calculate_ratio(numerator, denominator):
            try:
                return numerator / denominator
            except ZeroDivisionError:
                return 0

        ###################################
        tradetime = self.time_index[feat_index]
        prev_index = feat_index - 1
        previous_trade = {}
        while prev_index >= 0:
            if not np.isnan(self.feat.iloc[prev_index]['close']):
                previous_trade = self.feat.iloc[prev_index].to_dict()
                break
            prev_index -= 1
        # previous_trade = self.feat.iloc[feat_index-1].to_dict() if feat_index >= 1 else {}
        # previous_trade = self.feat.iloc[self.effect_feat_index].to_dict() if self.effect_feat_index >= 0 else {}
        cur_trade_buffer = deque()
        while self.trade_buffer and tradetime.time() >= self.trade_buffer[0]['datetime'].time():
            trade_info = self.trade_buffer.popleft()
            cur_trade_buffer.append(trade_info)

        if len(cur_trade_buffer) == 0:
            features = {
                'open': previous_trade.get('close', np.nan),
                'close': previous_trade.get('close', np.nan),
                'high': previous_trade.get('close', np.nan),
                'low': previous_trade.get('close', np.nan),
                'vwap': previous_trade.get('vwap', np.nan),
                'td_buy_num': 0, 'td_sell_num': 0,
                'td_buy_price': previous_trade.get('td_buy_price', np.nan),
                'td_sell_price': previous_trade.get('td_sell_price', np.nan),
                'td_buy_vol': 0, 'td_sell_vol': 0,
                # 'td_buy_price_std': 0.0, 'td_sell_price_std': 0.0,
                'td_vol': 0, 'td_price_std': np.nan
            }
        else:
            open, close = cur_trade_buffer[0]['trade_price'], cur_trade_buffer[-1]['trade_price']
            high, low = cur_trade_buffer[0]['trade_price'], cur_trade_buffer[0]['trade_price']
            td_buy_num, td_sell_num, td_buy_price_std, td_sell_price_std = 0, 0, 0, 0
            td_buy_price, td_sell_price,  td_buy_vol,  td_sell_vol = 0, 0, 0, 0
            # buy_price_sum, buy_price_sqsum = 0, 0
            # sell_price_sum, sell_price_sqsum = 0, 0
            price_list = []
            # 加入时间判断，保证在有新的成交数据输入到trade_buffer时，仍能正确地进行计算
            while cur_trade_buffer:
                trade_info = cur_trade_buffer.popleft()
                price = trade_info['trade_price']
                vol = trade_info['trade_volume']
                high = price if high < price else high
                low = price if low > price else low
                td_buy_num += (trade_info['side'] == 'B')
                td_sell_num += (trade_info['side'] == 'S')
                td_buy_price += ((trade_info['side'] == 'B') * price * vol)
                td_sell_price += ((trade_info['side'] == 'S') * price * vol)
                td_buy_vol += ((trade_info['side'] == 'B') * vol)
                td_sell_vol += ((trade_info['side'] == 'S') * vol)
                price_list.append(price)
                # price_sum += price
                # price_sqsum += price * price
            td_vol = td_buy_vol + td_sell_vol
            vwap = (td_buy_price + td_sell_price) / (td_buy_vol + td_sell_vol)

            if td_buy_vol > 0:
                td_buy_price = calculate_ratio(td_buy_price, td_buy_vol)
            else:
                td_buy_price =previous_trade.get('td_buy_price', np.nan)

            if td_sell_vol > 0:
                td_sell_price = calculate_ratio(td_sell_price, td_sell_vol)
            else:
                td_sell_price = previous_trade.get('td_sell_price', np.nan)

            td_num = (td_buy_num + td_sell_num)

            price_std = np.nan
            if td_num > 1:
                price_std = np.std(price_list, ddof=1)
                if price_std < 1e-7:
                    price_std = 0.0
                # variance = (price_sqsum - (price_sum * price_sum) / td_num) / (td_num - 1)
                # if variance < 0:
                #     price_std = 0.0
                # else:
                #     price_std = math.sqrt(variance)

            features = {
                'open': open,
                'close': close, #用快照数据获取close
                'high': high,
                'low': low,
                'td_buy_num': td_buy_num, 'td_sell_num': td_sell_num, 'td_buy_price': td_buy_price, 'td_sell_price': td_sell_price,
                'td_buy_vol': td_buy_vol, 'td_sell_vol': td_sell_vol,
                # 'td_buy_price_std': 0.0, 'td_sell_price_std': 0.0,
                'td_vol': td_vol, 'vwap': vwap, 'td_price_std': price_std
            }
        self.add_feature(feat_index, features)


    def build_entrust_features(self, feat_index):
        ##########   构建特征所需函数   ######
        def calculate_ratio(numerator, denominator):
            try:
                return numerator / denominator
            except ZeroDivisionError:
                return 0

        ###################################
        tradetime = self.time_index[feat_index]
        prev_index = feat_index - 1
        previous_entrust = {}
        while prev_index >= 0:
            if not np.isnan(self.feat.iloc[prev_index]['en_buy_price']):
                previous_entrust = self.feat.iloc[prev_index].to_dict()
                break
            prev_index -= 1
        # previous_entrust = self.feat.iloc[feat_index - 1].to_dict() if feat_index >= 1 else {}
        # previous_entrust = self.feat.iloc[self.effect_feat_index].to_dict() if self.effect_feat_index >= 0 else {}

        cur_entrust_buffer = deque()
        while self.entrust_buffer and tradetime.time() >= self.entrust_buffer[0]['datetime'].time():
            entrust_info = self.entrust_buffer.popleft()
            cur_entrust_buffer.append(entrust_info)

        if len(cur_entrust_buffer) == 0:
            features = {
                'en_buy_num': 0, 'en_sell_num': 0,
                'en_buy_price': previous_entrust.get('en_buy_price', np.nan),
                'en_sell_price': previous_entrust.get('en_sell_price', np.nan),
                'en_buy_vol': 0, 'en_sell_vol': 0, 'en_price_std': 0.0,
            }
        else:
            en_buy_num, en_sell_num = 0, 0
            en_buy_price, en_sell_price, en_buy_vol, en_sell_vol = 0, 0, 0, 0
            price_list = []
            # 加入时间判断，保证在有新的委托数据输入到entrust_buffer时，仍能正确地进行计算
            while cur_entrust_buffer:
                entrust_info = cur_entrust_buffer.popleft()
                price = entrust_info['order_price']
                vol = entrust_info['order_volume']

                en_buy_num += (entrust_info['side'] == 'B')
                en_sell_num += (entrust_info['side'] == 'S')
                en_buy_price += ((entrust_info['side'] == 'B') * price * vol)
                en_sell_price += ((entrust_info['side'] == 'S') * price * vol)
                en_buy_vol += ((entrust_info['side'] == 'B') * vol)
                en_sell_vol += ((entrust_info['side'] == 'S') * vol)
                price_list.append(price)
            if en_buy_vol > 0:
                en_buy_price = calculate_ratio(en_buy_price, en_buy_vol)
            else:
                en_buy_price = np.nan
            if en_sell_vol > 0:
                en_sell_price = calculate_ratio(en_sell_price, en_sell_vol)
            else:
                en_sell_price = np.nan

            en_num = (en_buy_num + en_sell_num)
            en_price_std = np.nan
            if en_num > 1:
                en_price_std = np.std(price_list, ddof=1)
                if en_price_std < 1e-7:
                    en_price_std = 0.0
                # variance = (price_sqsum - (price_sum * price_sum) / en_num) / (en_num - 1)
                # try:
                #     en_price_std = math.sqrt(variance)
                # except ValueError as e:
                #     en_price_std = 0

            features = {
                'en_buy_num': en_buy_num, 'en_sell_num': en_sell_num, 'en_buy_price': en_buy_price, 'en_sell_price': en_sell_price,
                'en_buy_vol': en_buy_vol, 'en_sell_vol': en_sell_vol, 'en_price_std': en_price_std,
            }
        self.add_feature(feat_index,  features)

    def build_cancel_features(self, feat_index):
        tradetime = self.time_index[feat_index]
        cur_cancel_buffer = deque()
        while self.cancel_buffer and tradetime.time() >= self.cancel_buffer[0]['datetime'].time():
            cancel_info = self.cancel_buffer.popleft()
            cur_cancel_buffer.append(cancel_info)

        if len(cur_cancel_buffer) == 0:
            features = {
                'cancel_buy_num': 0, 'cancel_sell_num': 0, 'cancel_buy_vol': 0, 'cancel_sell_vol': 0,
                'cancel_buy_time_range': 0.0, 'cancel_sell_time_range': 0.0,
                'cancel_buy_time_med': 0.0, 'cancel_sell_time_med': 0.0,
            }
        else:
            cancel_buy_num, cancel_sell_num, cancel_buy_vol, cancel_sell_vol = 0, 0, 0, 0
            buy_time_list, sell_time_list = [], []
            # 加入时间判断，保证在有新的撤销数据输入到cancel_buffer时，仍能正确地进行计算
            while cur_cancel_buffer:
                cancel_info = cur_cancel_buffer.popleft()
                vol = cancel_info['volume']
                time = cancel_info['datetime']
                try:
                    match_order = self.entrust_dict_by_appl_seq[cancel_info['appl_seq_num']]
                    delta_time = (time - match_order['datetime']).total_seconds() * 1000
                    # log.warning("当前撤单找到匹配的订单")
                except KeyError as e:
                    log.error(f"当前撤单无法找到匹配的订单{cancel_info['appl_seq_num']}")
                    delta_time = np.nan
                    # continue
                if cancel_info['side'] == 'B':
                    cancel_buy_num += 1
                    cancel_buy_vol += vol
                    buy_time_list.append(delta_time)
                elif cancel_info['side'] == 'S':
                    cancel_sell_num += 1
                    cancel_sell_vol += vol
                    sell_time_list.append(delta_time)
                else:
                    log.error("Unknown side")

            if len(buy_time_list) > 0:
                cancel_buy_time_range = (np.nanmax(buy_time_list) - np.nanmin(buy_time_list)).item()
                cancel_buy_time_med = np.nanmedian(buy_time_list).item()
            else:
                cancel_buy_time_range = 0
                cancel_buy_time_med = 0
            if len(sell_time_list) > 0:
                cancel_sell_time_range = (np.nanmax(sell_time_list) - np.nanmin(sell_time_list)).item()
                cancel_sell_time_med = np.nanmedian(sell_time_list).item()
            else:
                cancel_sell_time_range = 0
                cancel_sell_time_med = 0
            features = {
                'cancel_buy_num': cancel_buy_num, 'cancel_sell_num': cancel_sell_num, 'cancel_buy_vol': cancel_buy_vol, 'cancel_sell_vol': cancel_sell_vol,
                'cancel_buy_time_range': cancel_buy_time_range, 'cancel_sell_time_range': cancel_sell_time_range,
                'cancel_buy_time_med': cancel_buy_time_med, 'cancel_sell_time_med': cancel_sell_time_med,
            }
        self.add_feature(feat_index, features)
