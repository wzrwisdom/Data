#ifndef FEATURE_BUILDER_HPP
#define FEATURE_BUILDER_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <map>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <unordered_map>
#include <stdexcept>
#include <core/Utils.hpp>
#include <core/MultiDimData.hpp>
#include <core/MarketDataTypes.hpp>
#include <feature/featureBuilderBase.hpp>

using namespace std;
using namespace std::chrono;
using namespace MarketData;

class FeatureBuilder: public FeatureBuilderBase {
private:
    
    // 交易数据缓冲区
    deque<TradeData> trade_buffer_;
    // 委托数据缓冲区
    deque<OrderData> entrust_buffer_;
    // 撤单数据缓冲区
    deque<CancelData> cancel_buffer_;
    // 委托字典，以appl_seq_为键
    map<long, OrderData> entrust_dict_by_appl_seq_;


    // Helper functions
    int find_time_index(const system_clock::time_point& time);
    double calculate_wb(double bv_sum, double sv_sum, double last_wb);
    double calculate_ratio(double numerator, double denominator);
    double get_prev_feature(size_t iIdx, size_t tIdx, const string& feature_name);
    void build_transaction_features(size_t iIdx, size_t tIdx);
    void build_entrust_features(size_t iIdx, size_t tIdx);
    void build_cancel_features(size_t iIdx, size_t tIdx);
    double median(vector<double>& v);

public:
    FeatureBuilder(vector<string> instruments);

    Utils::Timestamp build_snap_features(const SnapData& data);
    void build_snap_features_byDDB(const SnapData& data, const Utils::Timestamp& time);
    void fill_snap_features(std::string symbol, const Utils::Timestamp& time);
    void add_transaction(const TradeData& data);
    void add_entrust(const OrderData& data);
    void build_other_features(std::string inst, Utils::Timestamp time);
};

#endif // FEATURE_BUILDER_HPP