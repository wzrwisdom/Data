#ifndef FEATURE_BUILDER_BASE_H
#define FEATURE_BUILDER_BASE_H

// 包含必要的头文件
#include <iostream>
#include <string>
#include <functional>
#include <algorithm>
#include <cmath>

#include <core/Utils.hpp>
#include <core/MultiDimData.hpp>


using namespace std;

class FeatureBuilderBase {
public:
    // 构造函数
    FeatureBuilderBase();

    // 析构函数
    virtual ~FeatureBuilderBase();

    function<vector<double>(const string&, int)> build_feature_extractor(const string& inst, const Utils::Timestamp& cur_time);

    void add_feature(const string& inst, const Utils::Timestamp& time, const unordered_map<string, double>& feature_info);

protected:
    // 时间索引
    vector<Utils::Timestamp> time_indices_;
    // 特征数据
    ThreeDimData feat_;
    // 起始索引
    size_t start_time_index_;
    // 前向填充的特征
    vector<string> ffill_feats_;
    // 填零的特征
    vector<string> fillzero_feats_;
};


#endif // FEATURE_BUILDER_BASE_H
