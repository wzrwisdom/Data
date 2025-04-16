#include "core/MultiDimData.hpp"
#include <cmath>


ThreeDimData::ThreeDimData(const std::vector<std::string>& instruments,
                            const std::vector<Utils::Timestamp>& timePoints,
                            const std::vector<std::string>& featureNames)
    : instruments_(instruments),
      timePoints_(timePoints),
      featureNames_(featureNames) {
    
    // 建立索引映射
    for (size_t i = 0; i < instruments.size(); ++i) {
        instrumentIndex_[instruments[i]] = i;
    }
    for (size_t i = 0; i < timePoints.size(); ++i) {
        timeIndex_[timePoints[i].to_string()] = i;
    }
    for (size_t i = 0; i < featureNames.size(); ++i) {
        featureIndex_[featureNames[i]] = i;
    }
    
    // 预分配三维数据空间
    data_.resize(instruments.size());
    for (auto& timeVec : data_) {
        timeVec.resize(timePoints.size());
        for (auto& featureVec : timeVec) {
            featureVec.resize(featureNames.size(), std::numeric_limits<double>::quiet_NaN());
        }
    }
}

size_t ThreeDimData::numInstruments() const { return instruments_.size(); }
size_t ThreeDimData::numTimePoints() const { return timePoints_.size(); }
size_t ThreeDimData::numFeatures() const { return featureNames_.size(); }

size_t ThreeDimData::getInstrumentIndex(const std::string& instrument) const {
    auto it = instrumentIndex_.find(instrument);
    if (it == instrumentIndex_.end()) {
        throw std::out_of_range("Instrument not found: " + instrument);
    }
    return it->second;
}

size_t ThreeDimData::getTimeIndex(const Utils::Timestamp& timePoint) const {
    auto it = timeIndex_.find(timePoint.to_string());
    if (it == timeIndex_.end()) {
        // throw std::out_of_range("Time point not found: " + timePoint.to_string());
        return -1;
    }
    return it->second;
}

// 获取特征索引
size_t ThreeDimData::getFeatureIndex(const std::string& featureName) const {
    // 在featureIndex_中查找featureName
    auto it = featureIndex_.find(featureName);
    // 如果找不到，抛出异常
    if (it == featureIndex_.end()) {
        // throw std::out_of_range("Feature not found: " + featureName);
        return -1;
    }
    // 返回特征索引
    return it->second;
}

double& ThreeDimData::at(size_t iIdx, size_t tIdx, size_t fIdx) {
    return data_[iIdx][tIdx][fIdx];
}

const double& ThreeDimData::at(size_t iIdx, size_t tIdx, size_t fIdx) const {
    return data_[iIdx][tIdx][fIdx];
}

double& ThreeDimData::at(size_t iIdx, size_t tIdx, const std::string& featureName) {
    return data_[iIdx][tIdx][getFeatureIndex(featureName)];
}

const double& ThreeDimData::at(size_t iIdx, size_t tIdx, const std::string& featureName) const {
    return data_[iIdx][tIdx][getFeatureIndex(featureName)];
}

double& ThreeDimData::at(const std::string& instrument, 
                         const Utils::Timestamp& timePoint, 
                         const std::string& featureName) {
    return data_[getInstrumentIndex(instrument)]
                [getTimeIndex(timePoint)]
                [getFeatureIndex(featureName)];
}

const double& ThreeDimData::at(const std::string& instrument, 
                               const Utils::Timestamp& timePoint, 
                               const std::string& featureName) const {
    return data_[getInstrumentIndex(instrument)]
                [getTimeIndex(timePoint)]
                [getFeatureIndex(featureName)];
}

// 重载()运算符，返回data_数组中第i行第j列第k个元素的引用
double& ThreeDimData::operator()(size_t i, size_t j, const std::string& feat_name) {
    return data_[i][j][getFeatureIndex(feat_name)];
}

const double& ThreeDimData::operator()(size_t i, size_t j, const std::string& feat_name) const {
    return data_[i][j][getFeatureIndex(feat_name)];
}

std::unordered_map<std::string, double>& ThreeDimData::operator()(size_t i, size_t j) {
    static std::unordered_map<std::string, double> featureMap;
    featureMap.clear();
    for (size_t fIdx = 0; fIdx < featureNames_.size(); ++fIdx) {
        featureMap[featureNames_[fIdx]] = data_[i][j][fIdx];
    }
    return featureMap;
}

const std::unordered_map<std::string, double>& ThreeDimData::operator()(size_t i, size_t j) const {
    static std::unordered_map<std::string, double> featureMap;
    featureMap.clear();
    for (size_t fIdx = 0; fIdx < featureNames_.size(); ++fIdx) {
        featureMap[featureNames_[fIdx]] = data_[i][j][fIdx];
    }
    return featureMap;
}

auto ThreeDimData::extractSubset(size_t iIdx, size_t tIdxStart, size_t tIdxEnd, size_t fIdx) const {
    // 检查边界
    if (iIdx >= instrumentIndex_.size() || 
        tIdxEnd >= timeIndex_.size() ||
        tIdxStart > tIdxEnd ||
        fIdx >= featureIndex_.size()) {
        throw std::out_of_range("Invalid dimension indices");
    }
    
    // 准备结果容器
    size_t numRows = tIdxEnd - tIdxStart + 1;
    std::vector<double> result(numRows, std::numeric_limits<double>::quiet_NaN());
    
    // 填充数据
    for (size_t i = tIdxStart; i <= tIdxEnd; ++i) {
        result[i - tIdxStart] = data_[iIdx][i][fIdx];
    }
    return result;
}

void ThreeDimData::setOneFeature(const std::string& instrument,
                                 const Utils::Timestamp& timePoint,
                                 const std::string& featureName,
                                 double value) {
    data_[getInstrumentIndex(instrument)]
        [getTimeIndex(timePoint)]
        [getFeatureIndex(featureName)] = value;
}

// 设置所有特征
void ThreeDimData::setAllFeatures(const std::string& instrument,
                                  const Utils::Timestamp& timePoint,
                                  const std::vector<double>& values) {
    // 如果值的数量不等于特征数量，抛出异常
    if (values.size() != featureNames_.size()) {
        throw std::invalid_argument("Values size doesn't match feature count");
    }
    // 获取仪器索引
    size_t iIdx = getInstrumentIndex(instrument);
    // 获取时间点索引
    size_t tIdx = getTimeIndex(timePoint);
    // 将值赋给数据
    data_[iIdx][tIdx] = values;
}

const std::string& ThreeDimData::getInstrumentName(size_t index) const {
    return instruments_.at(index);
}

const Utils::Timestamp& ThreeDimData::getTimePointName(size_t index) const {
    return timePoints_.at(index);
}

const std::string& ThreeDimData::getFeatureName(size_t index) const {
    return featureNames_.at(index);
}

void ThreeDimData::addInstrument(const std::string& instrument) {
    if (instrumentIndex_.count(instrument)) {
        return; // 已存在
    }
    
    instrumentIndex_[instrument] = instruments_.size();
    instruments_.push_back(instrument);
    
    // 扩展数据存储
    data_.emplace_back(timePoints_.size(), 
                     std::vector<double>(featureNames_.size(), 
                                       std::numeric_limits<double>::quiet_NaN()));
}

double ThreeDimData::featureMean(const std::string& featureName) const {
    size_t fIdx = getFeatureIndex(featureName);
    double sum = 0.0;
    size_t count = 0;
    
    for (const auto& timeVec : data_) {
        for (const auto& featureVec : timeVec) {
            double val = featureVec[fIdx];
            if (!std::isnan(val)) {
                sum += val;
                ++count;
            }
        }
    }
    
    return count > 0 ? sum / count : std::numeric_limits<double>::quiet_NaN();
}



