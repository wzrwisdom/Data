from FlowDataTradeSystem.factor.factors.base import Factor

from loguru import logger as log
from collections import deque
import yaml
import time
import pandas as pd
import numpy as np


class FactorBuilder:
    def __init__(self, feature_builder, config_filepath):
        self.feature_builder = feature_builder
        self.factors = []
        self.factor_names = []
        self.register_factors_by_config(config_filepath)
        self.history_facComb_bySym = {}
        self.results = None

        # 生成交易时间内的时间索引
        start_time = pd.Timestamp('09:30:03')
        end_time = pd.Timestamp('11:30:00')
        self.time_index = pd.date_range(start=start_time, end=end_time, freq='3s')
        start_time = pd.Timestamp('13:00:00')
        end_time = pd.Timestamp('14:57:00')
        self.time_index = self.time_index.append(pd.date_range(start=start_time, end=end_time, freq='3s'))
        
        # 生成存储因子的数组
        self.initialize_fac_matrix()
    
    def setFeatBuilder(self, featBuilder):
        self.feature_builder = featBuilder
    
    def initialize_fac_matrix(self):
        self.fac_results = pd.DataFrame(
            np.full((len(self.time_index), len(self.factor_names)), np.nan),
            index=self.time_index, columns=self.factor_names
        )
        self.ffill_facs = []

    def add_facComb(self, symbol, facComb):
        if symbol not in self.history_facComb_bySym.keys():
            self.history_facComb_bySym[symbol] = deque()
        else:
            self.history_facComb_bySym[symbol].append(facComb)

    def register_factors_by_config(self, config_file):
        """加载配置文件并注册因子"""
        with open(config_file, 'r') as f:
            config = yaml.safe_load(f)

        self.factor_names = list(config.keys())

        for factor_class in Factor._registry:
            exist_flag = factor_class.name in self.factor_names
            if exist_flag:
                kwargs = config[factor_class.name]
                if kwargs is not None:
                    self.factors.append(factor_class(**kwargs))
                else:
                    log.debug(f"因子{factor_class.name}没有参数配置")  # 实际运行需要注释
                    self.factors.append(factor_class())

    def add_factors(self, index, fac_results):
        cur_time = self.time_index[index]
        for name, value in fac_results.items():
            last_valid_time = self.fac_results[name].loc[:cur_time].last_valid_index()
            last_valid_index = self.time_index.get_indexer([last_valid_time])[0]
            # 判断特征是否存在空缺，将空缺部分进行填充
            if last_valid_time and (last_valid_index + 1 < index):
                fill_range = self.fac_results.loc[last_valid_time:cur_time, name]
                if name in self.ffill_facs:
                    self.fac_results.loc[last_valid_time:cur_time, name] = fill_range.ffill()
            
            self.fac_results.loc[cur_time, name] = value

    def compute_all_factors(self, time):
        """
        计算所有注册的因子
        :return:
        """
        results = {}
        # computation_times = {}

        for factor in self.factors:
            factor_name = factor.name

            # start_time = time.perf_counter()
            result = factor.compute(self.feature_builder.build_feature_extractor(time))
            # end_time = time.perf_counter()

            # 保存计算结果和耗时
            results[factor_name] = result
            # computation_times[factor_name] = end_time - start_time

        # # 打印或存储耗时信息
        # for factor_name, elapsed_time in computation_times.items():
        #     print(f"Factor '{factor_name}' computed in {elapsed_time:.4f} seconds")
        index = self.time_index.get_indexer([time])[0].item()
        self.add_factors(index, results)
        self.results = results
        return results

    def get_results(self):
        return self.results

    def preprocess(self, filepath, judge_col, no_winsorize_factors, factors):
        # 因子预处理的参数文件读取 可能不要放在这儿 避免重复加载
        with open(filepath, 'r') as f:
            params_dict = yaml.safe_load(f)

        fillZero_flag = factors[judge_col] is not None
        if not fillZero_flag:
            log.debug(f"因子{judge_col}为空")
            return None
        for factor_name in factors.keys():
            value = factors[factor_name]
            if (value is not None) and (factor_name in params_dict.keys()):
                params = params_dict[factor_name]
                if 'lower_quantile' in params.keys() and factor_name not in no_winsorize_factors:
                    lower_q = params['lower_quantile']
                    upper_q = params['upper_quantile']
                    if value < lower_q:
                        factors[factor_name] = lower_q
                    elif value > upper_q:
                        factors[factor_name] = upper_q
                if 'fac_mean' in params.keys() and 'fac_std' in params.keys():
                    fac_mean = params['fac_mean']
                    fac_std = params['fac_std']
                    factors[factor_name] = (factors[factor_name] - fac_mean) / fac_std

            if ((value is None) or (np.isnan(value))) and fillZero_flag:
                factors[factor_name] = 0

        return factors
