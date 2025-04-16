#ifndef FACTOR_BUILDER_H
#define FACTOR_BUILDER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <set>
#include <fstream>
#include <yaml-cpp/yaml.h> // 需要 yaml-cpp 库

#include "factor/base.hpp"
#include "feature/featureBuilderBase.hpp"
#include "factor/factorFactory.hpp"
#include "core/MultiDimData.hpp"


using namespace std::chrono;

class FactorBuilder {
public:
    explicit FactorBuilder(std::vector<std::string> instruments, std::shared_ptr<FeatureBuilderBase> fb, const std::string& configPath);
    
    void computeAllFactors(const std::string& instrument, const Utils::Timestamp& time);

    void add_factors(const std::string& instrument, const Utils::Timestamp& time, const unordered_map<string, double>& factor_info);
    
    // 获取计算结果
    const std::unordered_map<std::string, double>& getFactorResults() const;

private:
    std::shared_ptr<FeatureBuilderBase> feature_builder_;
    std::string configPath_;
    std::vector<string> factor_names_;
    std::vector<string> instruments_;
    // 前向填充的特征
    vector<string> ffill_feats_;
    // 时间索引
    vector<Utils::Timestamp> time_indices_;
    ThreeDimData fac_;

    std::unordered_map<std::string, std::shared_ptr<BaseFactor>> registeredFactors_;
    std::unordered_map<std::string, double> factorResults_;
    // 注册因子
    void loadFactorNamesFromConfig(const std::string& configPath);
    void initializeFacMatrix(); // 初始化因子矩阵
    void registerFactor(const std::string& name, std::shared_ptr<BaseFactor> factor);
};

#endif // FACTOR_BUILDER_H
