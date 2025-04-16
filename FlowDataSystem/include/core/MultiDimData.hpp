#ifndef MULTIDIMDATA_HPP
#define MULTIDIMDATA_HPP

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <algorithm>
#include <core/Utils.hpp>

class ThreeDimData {
private:
    // 索引映射
    std::vector<std::string> instruments_;
    std::vector<Utils::Timestamp> timePoints_;
    std::vector<std::string> featureNames_;
    
    // 索引查找表
    using InstrumentIndexMap = std::unordered_map<std::string, size_t>;
    using TimeIndexMap = std::unordered_map<std::string, size_t>;
    using FeatureIndexMap = std::unordered_map<std::string, size_t>;
    InstrumentIndexMap instrumentIndex_;   
    TimeIndexMap timeIndex_;
    FeatureIndexMap featureIndex_;
    
    using DataStorage = std::vector<std::vector<std::vector<double>>>;
    DataStorage data_;
    
public:
    // 默认构造函数
    ThreeDimData() = default;
    // 构造函数，预分配空间
    ThreeDimData(const std::vector<std::string>& instruments,
                const std::vector<Utils::Timestamp>& timePoints,
                const std::vector<std::string>& featureNames);
    
    // 获取维度信息
    size_t numInstruments() const;
    size_t numTimePoints() const;
    size_t numFeatures() const;
    
    // 获取索引
    size_t getInstrumentIndex(const std::string& instrument) const;
    size_t getTimeIndex(const Utils::Timestamp& timePoint) const;
    size_t getFeatureIndex(const std::string& featureName) const;
    
    // 数据访问方法
    double& at(size_t iIdx, size_t tIdx, size_t fIdx);
    const double& at(size_t iIdx, size_t tIdx, size_t fIdx) const;
    double& at(size_t iIdx, size_t tIdx, const std::string& featureName);
    const double& at(size_t iIdx, size_t tIdx, const std::string& featureName) const;
    double& at(const std::string& instrument, 
              const Utils::Timestamp& timePoint, 
              const std::string& featureName);
    const double& at(const std::string& instrument, 
                    const Utils::Timestamp& timePoint, 
                    const std::string& featureName) const;
    double& operator()(size_t i, size_t j, const std::string& featureName);
    const double& operator()(size_t i, size_t j, const std::string& featureName) const;
    std::unordered_map<std::string, double>& operator()(size_t i, size_t j);
    const std::unordered_map<std::string, double>& operator()(size_t i, size_t j) const;

    auto extractSubset(size_t iIdx, size_t tIdxStart, size_t tIdxEnd, size_t fIdx) const;
    
    // 数据修改方法
    void setOneFeature(const std::string& instrument,
        const Utils::Timestamp& timePoint,
        const std::string& featureName,
        double value);

    // 批量操作方法
    void setAllFeatures(const std::string& instrument,
                       const Utils::Timestamp& timePoint,
                       const std::vector<double>& values);
    
    // 获取标的物名称
    const std::string& getInstrumentName(size_t index) const;
    
    // 获取时间点名称
    const Utils::Timestamp& getTimePointName(size_t index) const;
    
    // 获取特征名称
    const std::string& getFeatureName(size_t index) const;
    
    // 添加新标的物（扩展功能）
    void addInstrument(const std::string& instrument);
    
    // 统计方法示例：计算某个特征在所有标的物和时间点的平均值
    double featureMean(const std::string& featureName) const;
    
};

#endif // MULTIDIMDATA_HPP
