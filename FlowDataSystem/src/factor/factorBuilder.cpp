#include "factor/factorBuilder.hpp"



FactorBuilder::FactorBuilder(std::vector<std::string> instruments, std::shared_ptr<FeatureBuilderBase> fb, const std::string &configPath):
    instruments_(instruments), configPath_(configPath), feature_builder_(fb) {
    // 读取配置文件，获取因子名称列表
    loadFactorNamesFromConfig(configPath_);
    // 初始化因子矩阵
    initializeFacMatrix();

    ffill_feats_ = {};

}

void FactorBuilder::initializeFacMatrix()
{
    // Initialize time index
    auto now = system_clock::now();
    auto today = system_clock::to_time_t(now);
    tm tm_today = *localtime(&today);
    
    // Morning session: 09:30:03 - 11:30:00
    tm_today.tm_hour = 9; tm_today.tm_min = 30; tm_today.tm_sec = 3;
    auto start_morning = system_clock::from_time_t(mktime(&tm_today));
    tm_today.tm_hour = 11; tm_today.tm_min = 30; tm_today.tm_sec = 0;
    auto end_morning = system_clock::from_time_t(mktime(&tm_today));
    
    // Afternoon session: 13:00:00 - 14:57:00
    tm_today.tm_hour = 13; tm_today.tm_min = 0; tm_today.tm_sec = 0;
    auto start_afternoon = system_clock::from_time_t(mktime(&tm_today));
    tm_today.tm_hour = 14; tm_today.tm_min = 57; tm_today.tm_sec = 0;
    auto end_afternoon = system_clock::from_time_t(mktime(&tm_today));
    
    // Create time index with 3-second intervals
    for (auto t = start_morning; t <= end_morning; t += seconds(3)) {
        time_indices_.push_back(t);
    }
    for (auto t = start_afternoon; t <= end_afternoon; t += seconds(3)) {
        time_indices_.push_back(t);
    }
    // Initialize feature matrix with NaN
    fac_ = ThreeDimData(instruments_, time_indices_, factor_names_);
}


void FactorBuilder::registerFactor(const std::string& name, std::shared_ptr<BaseFactor> factor) {
    registeredFactors_[name] = factor;
}


void FactorBuilder::loadFactorNamesFromConfig(const std::string& configPath) {
    std::ifstream fin(configPath);
    if (!fin.is_open()) {
        throw std::runtime_error("Failed to open file!");
    }
    
    YAML::Node config = YAML::LoadFile(configPath);
    for (const auto& factor: config) {
        std::string factor_name = factor.first.as<std::string>();
        factor_names_.push_back(factor_name);
        std::cout << "Factor: " << factor_name << std::endl;
        YAML::Node factor_config = factor.second;

        try {
            auto factor = FactorFactory::instance().create(factor_name);

            // 将 config 中键值对转成 unordered_map<string, double>
            std::unordered_map<std::string, double> param_map;
            for (const auto& param : factor_config) {
                param_map[param.first.as<std::string>()] = param.second.as<double>();
            }

            factor->setParams(param_map);
            this->registerFactor(factor_name, factor);
        } catch (const std::exception& e) {
            std::cerr << "Error registering factor: " << e.what() << std::endl;
        }

    }

}

void FactorBuilder::add_factors(const std::string& inst, const Utils::Timestamp& time, const unordered_map<string, double>& factor_info){
    size_t tIdx = fac_.getTimeIndex(time);
    size_t iIdx = fac_.getInstrumentIndex(inst);
    for (const auto& [name, value] : factor_info) {
        // Find last valid index for this feature
        size_t last_valid_index = -1;
        for (size_t i = tIdx - 1; i >= 0; --i) {
            if (!isnan(fac_(iIdx, i, name))) {
                last_valid_index = i;
                break;
            }
        }
        
        // Fill missing values
        if (last_valid_index != -1 && (last_valid_index + 1 < tIdx)) {
            if (find(ffill_feats_.begin(), ffill_feats_.end(), name) != ffill_feats_.end()) {
                // Forward fill
                double fill_value = fac_(iIdx, last_valid_index, name);
                for (int i = last_valid_index + 1; i < tIdx; ++i) {
                    fac_(iIdx, i, name) = fill_value;
                }
            } 
        }
        // Set current value
        fac_(iIdx, tIdx, name) = value;
    }
}


void FactorBuilder::computeAllFactors(const std::string& instrument, const Utils::Timestamp& time) {
    for (const auto& fname : factor_names_) {
        if (registeredFactors_.count(fname)) {
            auto factor = registeredFactors_[fname];
            auto builder = feature_builder_->build_feature_extractor(instrument, time);
            factor->compute(builder);
            factorResults_[fname] = factor->getResult();
        } else {
            std::cerr << "Factor not registered: " << fname << std::endl;
        }
    }
}



const std::unordered_map<std::string, double> &FactorBuilder::getFactorResults() const
{
    return factorResults_;
}
