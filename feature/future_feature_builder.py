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

    interval_ms = 500
    a = 1 if (elapsed_ms % interval_ms) > 0 else 0
    aligned_ms = (elapsed_ms // interval_ms + a) * interval_ms  # 向上取整
    aligned_time = base_time + pd.to_timedelta(aligned_ms, unit="ms")
    return aligned_time

class FutureFeatureBuilder:
    def __init__(self, vpp=300):
        self.value_per_point = vpp
        # self.snap_feat_index = 0 # 标记程序目前指标计算到哪儿
        self.start_feat_index = None # 标记程序启动时是从哪个指标开始写入的
        self.effect_feat_index = None
        self.ffill_feats = ['last', 'tot_vol', 's1', 'b1', 'sv1_sum', 'bv1_sum', 'bs_avg_price', 'vwap', 'total_turnover']
        self.fillzero_feats = ['vol', 'turnover']
        # 生成存储特征的数组
        start_time = pd.Timestamp('09:30:00')
        end_time = pd.Timestamp('11:30:00')
        self.time_index = pd.date_range(start=start_time, end=end_time, freq='500ms')
        start_time = pd.Timestamp('13:00:00')
        end_time = pd.Timestamp('14:57:00')
        self.time_index = self.time_index.append(pd.date_range(start=start_time, end=end_time, freq='500ms'))

        feat_cols = ['last', 'vol', 'tot_vol', 's1', 'b1', 'sv1_sum', 'bv1_sum', 'bs_avg_price', 'vwap', 'total_turnover', 'turnover']
        self.feat = pd.DataFrame(
            np.full( (len(self.time_index), len(feat_cols)), np.nan),
            index=self.time_index, columns=feat_cols
        )

    def build_feature_extractor(self, cur_time):
        end_index = self.time_index.get_indexer([get_aligned_time(cur_time)])[0]+1
        def get_recent_features(name, window):
            if name not in self.feat.columns:
                log.error(f"特征类中没有{name}的数据")
            if window is None:
                start_index = 0
            else:
                start_index = end_index - window if end_index - window >=0 else 0
            start_index = max(self.start_feat_index, start_index)
            return self.feat.iloc[start_index:end_index][name].reset_index(drop=True)
        return get_recent_features

    def add_feature(self, index, feature_info):
        # if timestamp not in self.history_time:
        #     log.warning("特征对应的时间和 history_time中的最近时间不匹配") # 后续可以注释
        cur_time = self.feat.index[index]
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
            if np.isnan(np.float64(value)):
                pass
            else:
                self.feat.loc[cur_time, name] = value
        self.effect_feat_index = self.time_index.get_indexer([cur_time])[0]


    def build_snap_features(self, data):
        features = {}
        cur_time = get_aligned_time(pd.Timestamp(datetime.now()).normalize() + pd.to_timedelta(data['datetime'].time().strftime("%H:%M:%S.%f")))
        snap_feat_index = self.time_index.get_indexer([get_aligned_time(cur_time)])[0].item()
        if self.start_feat_index is None:
            self.start_feat_index = snap_feat_index

        if (snap_feat_index == 0) or (self.effect_feat_index is None):
            turnover = data['total_turnover']
            total_vol = data['vol']
            vol = total_vol
            try:
                vwap = turnover / (self.value_per_point * vol)
            except:
                vwap = data['last']
        else:
            previous_feat = self.feat.iloc[self.effect_feat_index]  
            turnover = data['total_turnover'] - previous_feat['total_turnover'].item()
            total_vol = data['vol']
            vol = total_vol - previous_feat['tot_vol'].item()
            try:
                vwap = turnover / (300 * vol)
            except:
                vwap = previous_feat['vwap'].item() if not np.isnan(previous_feat['vwap']) else data['last']

        features.update({
            'last': data['last'],
            'vol': vol,
            'tot_vol': total_vol,
            's1': data['s1'],
            'b1': data['b1'],
            'sv1_sum': data['sv1_sum'],
            'bv1_sum': data['bv1_sum'],
            'bs_avg_price': data['bs_avg_price'],
            'vwap': vwap,
            'total_turnover': data['total_turnover'],
            'turnover': turnover
        })
        self.add_feature(snap_feat_index, features)
        return self.time_index[snap_feat_index]
