#include <feature/futureFeatureBuilder.hpp>


FutureFeatureBuilder::FutureFeatureBuilder(vector<string> instruments, unordered_map<string, int> vpp) : value_per_point_(vpp), effect_time_index_(-1) {
    // Initialize time index
    auto now = system_clock::now();
    auto today = system_clock::to_time_t(now);
    tm tm_today = *localtime(&today);
    // Morning session: 09:30:03 - 11:30:00
    tm_today.tm_hour = 9; tm_today.tm_min = 30; tm_today.tm_sec = 0;
    auto start_morning = system_clock::from_time_t(mktime(&tm_today));
    tm_today.tm_hour = 11; tm_today.tm_min = 30; tm_today.tm_sec = 0;
    auto end_morning = system_clock::from_time_t(mktime(&tm_today));
    
    // Afternoon session: 13:00:00 - 14:57:00
    tm_today.tm_hour = 13; tm_today.tm_min = 0; tm_today.tm_sec = 0;
    auto start_afternoon = system_clock::from_time_t(mktime(&tm_today));
    tm_today.tm_hour = 14; tm_today.tm_min = 57; tm_today.tm_sec = 0;
    auto end_afternoon = system_clock::from_time_t(mktime(&tm_today));
    
    // Create time index with 3-second intervals
    for (auto t = start_morning; t <= end_morning; t += milliseconds(500)) {
        time_indices_.push_back(t);
    }
    for (auto t = start_afternoon; t <= end_afternoon; t += milliseconds(500)) {
        time_indices_.push_back(t);
    }

    vector<string> feat_cols = {"last", "vol", "tot_vol", "s1", "b1", "sv1_sum", "bv1_sum", "bs_avg_price", "vwap", "total_turnover", "turnover"};
    // Initialize feature matrix with NaN
    feat_ = ThreeDimData(instruments, time_indices_, feat_cols);

    // Set fill methods
    ffill_feats_ = {"last", "tot_vol", "s1", "b1", "sv1_sum", "bv1_sum", "bs_avg_price", "vwap", "total_turnover"};
    fillzero_feats_ = {"vol", "turnover"};

    start_time_index_ = -1;
}


Utils::Timestamp FutureFeatureBuilder::build_snap_features(const FutureSnapData& data) {
    auto aligned_time = data.timestamp.align_time(3000);
    size_t time_index = feat_.getTimeIndex(aligned_time);
    size_t inst_index = feat_.getInstrumentIndex(data.symbol);
    if (time_index == -1) throw runtime_error("Invalid time index");

    if (start_time_index_ == -1) {
        start_time_index_ = time_index;
    }

    double turnover, total_vol, vol, vwap;
    unordered_map<string, double> features;

    if (time_index == -1 || effect_time_index_ == -1) {
        turnover = data.total_turnover;
        total_vol = data.vol;
        vol = data.vol;
        try {
            vwap = turnover / (value_per_point_[data.symbol] * vol);
        } catch (...) {
            vwap = data.last;
        }
    } else {
        unordered_map<string, double> previous_feat;
        previous_feat = feat_(inst_index, effect_time_index_);
        turnover = feat_(inst_index, time_index, "total_turnover") - previous_feat["total_turnover"];
        total_vol = feat_(inst_index, time_index, "vol"); 
        vol = total_vol - previous_feat["vol"];
        try {
            vwap = turnover / (value_per_point_[data.symbol] * vol);
        } catch (...) {
            vwap = data.last;
        }
    }

    features = {
        {"last", data.last},
        {"vol", vol},
        {"tot_vol", total_vol},
        {"s1", data.s1},
        {"b1", data.b1},
        {"sv1_sum", data.sv1_sum},
        {"bv1_sum", data.bv1_sum},
        {"bs_avg_price", data.bs_avg_price},
        {"vwap", vwap},
        {"total_turnover", data.total_turnover},
        {"turnover", turnover}
    };

    add_feature(data.symbol, aligned_time, features);
    return aligned_time;
}
