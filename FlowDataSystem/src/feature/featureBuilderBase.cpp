#include "feature/featureBuilderBase.hpp"

using namespace std;

// 构造函数
FeatureBuilderBase::FeatureBuilderBase() {
    // 可以在这里初始化基类的成员变量（如果有）
    cout << "FeatureBuilderBase constructed." << endl;
}

// 析构函数
FeatureBuilderBase::~FeatureBuilderBase() {
    // 可以在这里释放资源（如果有）
    cout << "FeatureBuilderBase destructed." << std::endl;
}

// 构建特征提取器
function<vector<double>(const string&, int)> FeatureBuilderBase::build_feature_extractor(const string& inst, const Utils::Timestamp& cur_time) {
    // Find the aligned time and calculate the end index
    Utils::Timestamp aligned_time = cur_time.align_time(3000);
    if (aligned_time < time_indices_.front() || aligned_time > time_indices_.back()) {
        throw invalid_argument("Time out of valid range.");
    }
    size_t inst_index = feat_.getInstrumentIndex(inst);
    auto it = find(time_indices_.begin(), time_indices_.end(), aligned_time);
    int end_index = distance(time_indices_.begin(), it) + 1;

    // Return a lambda function to get recent features
    return [this, inst_index, end_index](const string& name, int window) -> vector<double> {
        // Check if the feature exists
        if (this->feat_.getFeatureIndex(name) == -1) {
            throw invalid_argument("Feature not found: " + name);
        }

        // Determine the start index
        int start_index = (window == -1) ? 0 : max(0, end_index - window);
        start_index = std::max(static_cast<int>(start_time_index_), start_index);

        // Extract the feature data
        vector<double> result;
        for (int i = start_index; i < end_index; ++i) {
            result.push_back(this->feat_(inst_index, i, name));
        }
        return result;
    };
}


void FeatureBuilderBase::add_feature(const string & inst, const Utils::Timestamp& time, const unordered_map<string, double>& feature_info) {
    size_t tIdx = feat_.getTimeIndex(time);
    size_t iIdx = feat_.getInstrumentIndex(inst);
    for (const auto& [name, value] : feature_info) {
        // Find last valid index for this feature
        size_t last_valid_index = -1;
        for (size_t i = tIdx - 1; i >= 0; --i) {
            if (!isnan(feat_(iIdx, i, name))) {
                last_valid_index = i;
                break;
            }
        }
        
        // Fill missing values
        if (last_valid_index != -1 && (last_valid_index + 1 < tIdx)) {
            if (find(ffill_feats_.begin(), ffill_feats_.end(), name) != ffill_feats_.end()) {
                // Forward fill
                double fill_value = feat_(iIdx, last_valid_index, name);
                for (int i = last_valid_index + 1; i < tIdx; ++i) {
                    feat_(iIdx, i, name) = fill_value;
                }
            } else if (find(fillzero_feats_.begin(), fillzero_feats_.end(), name) != fillzero_feats_.end()) {
                // Fill with zero
                for (int i = last_valid_index + 1; i < tIdx; ++i) {
                    feat_(iIdx, i, name) = 0.0;
                }
            } else if (name == "open" || name == "high" || name == "low") {
                // Fill with previous close
                double close_value = feat_(iIdx, last_valid_index, "close");
                for (int i = last_valid_index + 1; i < tIdx; ++i) {
                    feat_(iIdx, i, name) = close_value;
                }
            }
        }
        
        // Set current value
        feat_(iIdx, tIdx, name) = value;
    }
}
