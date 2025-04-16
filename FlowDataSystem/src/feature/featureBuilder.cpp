#include <feature/featureBuilder.hpp>


FeatureBuilder::FeatureBuilder(vector<string> instruments) {
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
    
    // Initialize feature columns
    vector<string> feat_cols = {
        "s1", "b1", "s5", "b5", "s10", "b10",
        "sv1_sum", "bv1_sum", "ssv1_sum", "bbv1_sum", "sv5_sum", "bv5_sum", 
        "bbv5_sum", "ssv5_sum", "sv10_sum", "bv10_sum", "ssv10_sum", "bbv10_sum",
        "wb1", "wb5", "wb10", "bs_avg_price",
        "open", "close", "high", "low", "vwap",
        "td_buy_num", "td_sell_num", "td_buy_price", "td_sell_price", 
        "td_buy_vol", "td_sell_vol", "td_vol", "td_price_std",
        "en_buy_num", "en_sell_num", "en_buy_price", "en_sell_price", 
        "en_buy_vol", "en_sell_vol", "en_price_std",
        "cancel_buy_num", "cancel_sell_num", "cancel_buy_vol", "cancel_sell_vol",
        "cancel_buy_time_range", "cancel_sell_time_range", 
        "cancel_buy_time_med", "cancel_sell_time_med"
    };
    
    // Initialize feature matrix with NaN
    feat_ = ThreeDimData(instruments, time_indices_, feat_cols);
    
    // Set fill methods
    ffill_feats_ = {
        "s1", "b1", "s5", "b5", "s10", "b10",
        "sv1_sum", "bv1_sum", "ssv1_sum", "bbv1_sum", "sv5_sum", "bv5_sum", 
        "bbv5_sum", "ssv5_sum", "sv10_sum", "bv10_sum", "ssv10_sum", "bbv10_sum",
        "wb1", "wb5", "wb10", "bs_avg_price",
        "close", "vwap"
    };
    
    fillzero_feats_ = {
        "td_buy_num", "td_sell_num", "td_buy_vol", "td_sell_vol", "td_vol",
        "en_buy_num", "en_sell_num", "en_buy_vol", "en_sell_vol",
        "cancel_buy_num", "cancel_sell_num", "cancel_buy_vol", "cancel_sell_vol"
    };
    
    start_time_index_ = -1;
}

// Main feature building functions
Utils::Timestamp FeatureBuilder::build_snap_features(const SnapData& data) {
    // 对时间戳进行对齐
    auto aligned_time = data.timestamp.align_time(3000);
    size_t time_index = feat_.getTimeIndex(aligned_time);
    size_t inst_index = feat_.getInstrumentIndex(data.symbol);
    if (time_index == -1) throw runtime_error("Invalid time index"); // Return a default timestamp
    
    if (start_time_index_ == -1) {
        start_time_index_ = time_index;
    }
    
    unordered_map<string, double> features;
    
    // Basic price and volume features
    features["s1"] = data.ask_prices[0];
    features["s5"] = data.ask_prices[4];
    features["s10"] = data.ask_prices[9];
    features["b1"] = data.bid_prices[0];
    features["b5"] = data.bid_prices[4];
    features["b10"] = data.bid_prices[9];
    
    features["sv1_sum"] = data.ask_volumes[0];
    features["bv1_sum"] = data.bid_volumes[0];
    features["ssv1_sum"] = data.ask_prices[0] * data.ask_volumes[0];
    features["bbv1_sum"] = data.bid_prices[0] * data.bid_volumes[0];
    
    // Calculate 5-level and 10-level sums
    double sv5_sum = 0, bv5_sum = 0, ssv5_sum = 0, bbv5_sum = 0;
    for (int i = 0; i < 5; ++i) {
        sv5_sum += data.ask_volumes[i];
        bv5_sum += data.bid_volumes[i];
        ssv5_sum += data.ask_prices[i] * data.ask_volumes[i];
        bbv5_sum += data.bid_prices[i] * data.bid_volumes[i];
    }
    
    double sv10_sum = sv5_sum, bv10_sum = bv5_sum;
    double ssv10_sum = ssv5_sum, bbv10_sum = bbv5_sum;
    for (int i = 5; i < 10; ++i) {
        sv10_sum += data.ask_volumes[i];
        bv10_sum += data.bid_volumes[i];
        ssv10_sum += data.ask_prices[i] * data.ask_volumes[i];
        bbv10_sum += data.bid_prices[i] * data.bid_volumes[i];
    }
    
    features["sv5_sum"] = sv5_sum;
    features["bv5_sum"] = bv5_sum;
    features["ssv5_sum"] = ssv5_sum;
    features["bbv5_sum"] = bbv5_sum;
    features["sv10_sum"] = sv10_sum;
    features["bv10_sum"] = bv10_sum;
    features["ssv10_sum"] = ssv10_sum;
    features["bbv10_sum"] = bbv10_sum;
    
    // Calculate wb features
    features["wb1"] = calculate_wb(features["bv1_sum"], features["sv1_sum"], 
                                    get_prev_feature(inst_index, time_index, "wb1"));
    features["wb5"] = calculate_wb(bv5_sum, sv5_sum, 
                                    get_prev_feature(inst_index, time_index, "wb5"));
    features["wb10"] = calculate_wb(bv10_sum, sv10_sum, 
                                    get_prev_feature(inst_index, time_index, "wb10"));
    
    features["bs_avg_price"] = (data.ask_prices[0] + data.bid_prices[0]) / 2;
    
    add_feature(data.symbol, aligned_time, features);
    return time_indices_[time_index];
}

void FeatureBuilder::build_snap_features_byDDB(const SnapData& data, const Utils::Timestamp& time) {
    auto aligned_time = data.timestamp.align_time(3000);
    size_t time_index = feat_.getTimeIndex(aligned_time);
    size_t inst_index = feat_.getInstrumentIndex(data.symbol);


    if (std::isnan(feat_(inst_index, time_index, "s1"))) {
        double sv1_sum = data.ask_volumes[0];
        double bv1_sum = data.bid_volumes[0];

        std::unordered_map<std::string, double> features = {
            {"s1", data.ask_prices[0]}, {"b1", data.bid_prices[0]},
            {"s5", data.ask_prices[4]}, {"b5", data.bid_prices[4]},
            {"s10", data.ask_prices[9]}, {"b10", data.bid_prices[9]},
            {"sv1_sum", sv1_sum}, {"bv1_sum", bv1_sum}
        };

        double sv5_sum = 0, bv5_sum = 0, ssv5_sum = 0, bbv5_sum = 0;
        for (int i = 0; i < 5; ++i) {
            sv5_sum += data.ask_volumes[i];
            bv5_sum += data.bid_volumes[i];
            ssv5_sum += data.ask_volumes[i] * data.ask_prices[i];
            bbv5_sum += data.bid_volumes[i] * data.bid_prices[i];
        }

        double sv10_sum = sv5_sum, bv10_sum = bv5_sum;
        double ssv10_sum = ssv5_sum, bbv10_sum = bbv5_sum;
        for (int i = 5; i < 10; ++i) {
            sv10_sum += data.ask_volumes[i];
            bv10_sum += data.bid_volumes[i];
            ssv10_sum += data.ask_volumes[i] * data.ask_prices[i];
            bbv10_sum += data.bid_volumes[i] * data.bid_prices[i];
        }

        double wb1 = calculate_wb(bv1_sum, sv1_sum, get_prev_feature(inst_index, time_index, "wb1"));
        double wb5 = calculate_wb(bv5_sum, sv5_sum, get_prev_feature(inst_index, time_index, "wb5"));
        double wb10 = calculate_wb(bv10_sum, sv10_sum, get_prev_feature(inst_index, time_index, "wb10"));
        double bs_avg_price = (data.ask_prices[0] + data.bid_prices[0]) / 2;

        features.insert({
            {"sv5_sum", sv5_sum}, {"bv5_sum", bv5_sum},
            {"ssv5_sum", ssv5_sum}, {"bbv5_sum", bbv5_sum},
            {"sv10_sum", sv10_sum}, {"bv10_sum", bv10_sum},
            {"ssv10_sum", ssv10_sum}, {"bbv10_sum", bbv10_sum},
            {"wb1", wb1}, {"wb5", wb5}, {"wb10", wb10}, {"bs_avg_price", bs_avg_price}
        });

        add_feature(data.symbol, aligned_time, features);
    }
}

void FeatureBuilder::fill_snap_features(std::string symbol, const Utils::Timestamp& time) {
    auto aligned_time = time.align_time(3000);
    size_t time_index = feat_.getTimeIndex(aligned_time);
    size_t inst_index = feat_.getInstrumentIndex(symbol);

    if (time_index > 0 && std::isnan(feat_(inst_index, time_index, "s1")) && !std::isnan(feat_(inst_index, time_index - 1, "s1"))) {
        std::vector<std::string> feat_cols = {
            "s1", "b1", "s5", "b5", "s10", "b10",
            "sv1_sum", "bv1_sum", "ssv1_sum", "bbv1_sum", "sv5_sum", "bv5_sum",
            "bbv5_sum", "ssv5_sum", "sv10_sum", "bv10_sum", "ssv10_sum", "bbv10_sum",
            "wb1", "wb5", "wb10", "bs_avg_price"
        };

        for (const auto& col : feat_cols) {
            feat_(inst_index, time_index, col) = feat_(inst_index, time_index - 1, col);
        }
    }
}

void FeatureBuilder::add_transaction(const TradeData& data) {
    if (data.exec_type == 'D') {
        // This is a cancel
        CancelData cancel_data;
        cancel_data.symbol = data.symbol;
        cancel_data.timestamp = data.timestamp;
        cancel_data.apply_seq_num = (data.bid_apply_seq_num == 0) ? data.ask_apply_seq_num : data.bid_apply_seq_num;
        cancel_data.side = (data.bid_apply_seq_num == 0) ? 'S' : 'B';
        cancel_data.volume = data.volume;
        cancel_buffer_.push_back(cancel_data);  
    } else {
        // This is a trade
        trade_buffer_.push_back(data);
    }
}

void FeatureBuilder::add_entrust(const OrderData& data) {
    if (data.order_type == 'D') {
        // This is a cancel
        CancelData cancel_data;
        cancel_data.symbol = data.symbol;
        cancel_data.timestamp = data.timestamp;
        cancel_data.apply_seq_num = data.apply_seq_num;
        cancel_data.side = data.side;
        cancel_data.volume = data.volume;
        cancel_buffer_.push_back(cancel_data);
    } else {
        // This is an order
        entrust_dict_by_appl_seq_[data.orig_order_no] = data;
        entrust_buffer_.push_back(data);
    }
}

void FeatureBuilder::build_other_features(std::string inst, Utils::Timestamp time) {
    size_t time_index = feat_.getTimeIndex(time);
    if (time_index == -1) return;
    
    size_t inst_index = feat_.getInstrumentIndex(inst);
    build_transaction_features(inst_index, time_index);
    build_entrust_features(inst_index, time_index);
    build_cancel_features(inst_index, time_index);
}

// Helper functions
int FeatureBuilder::find_time_index(const system_clock::time_point& time) {
    auto it = lower_bound(time_indices_.begin(), time_indices_.end(), time);
    if (it != time_indices_.end() && *it == time) {
        return distance(time_indices_.begin(), it);
    }
    return -1;
}

double FeatureBuilder::calculate_wb(double bv_sum, double sv_sum, double last_wb) {
    if (bv_sum + sv_sum == 0) return last_wb;
    return (bv_sum - sv_sum) / (bv_sum + sv_sum);
}

double FeatureBuilder::calculate_ratio(double numerator, double denominator) {
    if (denominator == 0) return 0;
    return numerator / denominator;
}

double FeatureBuilder::get_prev_feature(size_t iIdx, size_t tIdx, const string& feature_name) {
    for (int i = tIdx - 1; i >= 0; --i) {
        auto v = feat_(iIdx, i, feature_name);
        if (!isnan(v)) {
            return v;
        }
    }
    return 0.0; // Default value if no previous value found
}


void FeatureBuilder::build_transaction_features(size_t iIdx, size_t tIdx) {
    auto tradetime = time_indices_[tIdx];
    unordered_map<string, double> previous_trade;
    
    // Find previous valid trade
    for (int i = tIdx - 1; i >= 0; --i) {
        if (!isnan(feat_(iIdx, i, "close"))) {
            previous_trade = feat_(iIdx, i);
            break;
        }
    }
    
    deque<TradeData> cur_trade_buffer;
    while (!trade_buffer_.empty() && 
            trade_buffer_.front().timestamp <= tradetime) {
        cur_trade_buffer.push_back(trade_buffer_.front());
        trade_buffer_.pop_front();
    }
    
    unordered_map<string, double> features;
    
    if (cur_trade_buffer.empty()) {
        features = {
            {"open", previous_trade.count("close") ? previous_trade["close"] : numeric_limits<double>::quiet_NaN()},
            {"close", previous_trade.count("close") ? previous_trade["close"] : numeric_limits<double>::quiet_NaN()},
            {"high", previous_trade.count("close") ? previous_trade["close"] : numeric_limits<double>::quiet_NaN()},
            {"low", previous_trade.count("close") ? previous_trade["close"] : numeric_limits<double>::quiet_NaN()},
            {"vwap", previous_trade.count("vwap") ? previous_trade["vwap"] : numeric_limits<double>::quiet_NaN()},
            {"td_buy_num", 0}, {"td_sell_num", 0},
            {"td_buy_price", previous_trade.count("td_buy_price") ? previous_trade["td_buy_price"] : numeric_limits<double>::quiet_NaN()},
            {"td_sell_price", previous_trade.count("td_sell_price") ? previous_trade["td_sell_price"] : numeric_limits<double>::quiet_NaN()},
            {"td_buy_vol", 0}, {"td_sell_vol", 0},
            {"td_vol", 0}, {"td_price_std", numeric_limits<double>::quiet_NaN()}
        };
    } else {
        double open = cur_trade_buffer.front().price;
        double close = cur_trade_buffer.back().price;
        double high = open, low = open;
        
        int td_buy_num = 0, td_sell_num = 0;
        double td_buy_price = 0, td_sell_price = 0;
        long td_buy_vol = 0, td_sell_vol = 0;
        vector<double> price_list;
        
        while (!cur_trade_buffer.empty()) {
            auto trade = cur_trade_buffer.front();
            cur_trade_buffer.pop_front();
            
            double price = trade.price;
            long vol = trade.volume;
            
            high = max(high, price);
            low = min(low, price);
            
            if (trade.side == 'B') {
                td_buy_num++;
                td_buy_price += price * vol;
                td_buy_vol += vol;
            } else {
                td_sell_num++;
                td_sell_price += price * vol;
                td_sell_vol += vol;
            }
            price_list.push_back(price);
        }
        
        long td_vol = td_buy_vol + td_sell_vol;
        double vwap = (td_buy_price + td_sell_price) / td_vol;
        
        if (td_buy_vol > 0) {
            td_buy_price = calculate_ratio(td_buy_price, td_buy_vol);
        } else {
            td_buy_price = previous_trade.count("td_buy_price") ? 
                previous_trade["td_buy_price"] : numeric_limits<double>::quiet_NaN();
        }
        
        if (td_sell_vol > 0) {
            td_sell_price = calculate_ratio(td_sell_price, td_sell_vol);
        } else {
            td_sell_price = previous_trade.count("td_sell_price") ? 
                previous_trade["td_sell_price"] : numeric_limits<double>::quiet_NaN();
        }
        
        double price_std = numeric_limits<double>::quiet_NaN();
        if (price_list.size() > 1) {
            double sum = accumulate(price_list.begin(), price_list.end(), 0.0);
            double mean = sum / price_list.size();
            double sq_sum = inner_product(price_list.begin(), price_list.end(), 
                                            price_list.begin(), 0.0);
            double stdev = sqrt((sq_sum - mean * mean * price_list.size()) / (price_list.size() - 1));
            if (stdev < 1e-7) stdev = 0.0;
            price_std = stdev;
        }
        
        features = {
            {"open", open}, {"close", close}, {"high", high}, {"low", low},
            {"td_buy_num", static_cast<double>(td_buy_num)},
            {"td_sell_num", static_cast<double>(td_sell_num)},
            {"td_buy_price", td_buy_price},
            {"td_sell_price", td_sell_price},
            {"td_buy_vol", static_cast<double>(td_buy_vol)},
            {"td_sell_vol", static_cast<double>(td_sell_vol)},
            {"td_vol", static_cast<double>(td_vol)},
            {"vwap", vwap},
            {"td_price_std", price_std}
        };
    }
    
    add_feature(feat_.getInstrumentName(iIdx), tradetime, features);
}

void FeatureBuilder::build_entrust_features(size_t iIdx, size_t tIdx) {
    auto tradetime = time_indices_[tIdx];
    unordered_map<string, double> previous_entrust;
    
    // Find previous valid entrust
    for (int i = tIdx - 1; i >= 0; --i) {
        if (!isnan(feat_(iIdx, i, "en_buy_price"))) {
            previous_entrust = feat_(iIdx, i);
            break;
        }
    }
    
    deque<OrderData> cur_entrust_buffer;
    while (!entrust_buffer_.empty() && 
            entrust_buffer_.front().timestamp <= tradetime) {
        cur_entrust_buffer.push_back(entrust_buffer_.front());
        entrust_buffer_.pop_front();
    }
    
    unordered_map<string, double> features;
    
    if (cur_entrust_buffer.empty()) {
        features = {
            {"en_buy_num", 0}, {"en_sell_num", 0},
            {"en_buy_price", previous_entrust.count("en_buy_price") ? 
                previous_entrust["en_buy_price"] : numeric_limits<double>::quiet_NaN()},
            {"en_sell_price", previous_entrust.count("en_sell_price") ? 
                previous_entrust["en_sell_price"] : numeric_limits<double>::quiet_NaN()},
            {"en_buy_vol", 0}, {"en_sell_vol", 0}, 
            {"en_price_std", 0.0}
        };
    } else {
        int en_buy_num = 0, en_sell_num = 0;
        double en_buy_price = 0, en_sell_price = 0;
        long en_buy_vol = 0, en_sell_vol = 0;
        vector<double> price_list;
        
        while (!cur_entrust_buffer.empty()) {
            auto entrust = cur_entrust_buffer.front();
            cur_entrust_buffer.pop_front();
            
            double price = entrust.price;
            long vol = entrust.volume;
            
            if (entrust.side == 'B') {
                en_buy_num++;
                en_buy_price += price * vol;
                en_buy_vol += vol;
            } else {
                en_sell_num++;
                en_sell_price += price * vol;
                en_sell_vol += vol;
            }
            price_list.push_back(price);
        }
        
        if (en_buy_vol > 0) {
            en_buy_price = calculate_ratio(en_buy_price, en_buy_vol);
        } else {
            en_buy_price = numeric_limits<double>::quiet_NaN();
        }
        
        if (en_sell_vol > 0) {
            en_sell_price = calculate_ratio(en_sell_price, en_sell_vol);
        } else {
            en_sell_price = numeric_limits<double>::quiet_NaN();
        }
        
        double en_price_std = numeric_limits<double>::quiet_NaN();
        if (price_list.size() > 1) {
            double sum = accumulate(price_list.begin(), price_list.end(), 0.0);
            double mean = sum / price_list.size();
            double sq_sum = inner_product(price_list.begin(), price_list.end(), 
                                            price_list.begin(), 0.0);
            double stdev = sqrt((sq_sum - mean * mean * price_list.size()) / (price_list.size() - 1));
            if (stdev < 1e-7) stdev = 0.0;
            en_price_std = stdev;
        }
        
        features = {
            {"en_buy_num", static_cast<double>(en_buy_num)},
            {"en_sell_num", static_cast<double>(en_sell_num)},
            {"en_buy_price", en_buy_price},
            {"en_sell_price", en_sell_price},
            {"en_buy_vol", static_cast<double>(en_buy_vol)},
            {"en_sell_vol", static_cast<double>(en_sell_vol)},
            {"en_price_std", en_price_std}
        };
    }
    
    add_feature(feat_.getInstrumentName(iIdx), tradetime, features);
}

void FeatureBuilder::build_cancel_features(size_t iIdx, size_t tIdx) {
    auto tradetime = time_indices_[tIdx];
    deque<CancelData> cur_cancel_buffer;
    
    while (!cancel_buffer_.empty() && 
            cancel_buffer_.front().timestamp <= tradetime) {
        cur_cancel_buffer.push_back(cancel_buffer_.front());
        cancel_buffer_.pop_front();
    }
    
    unordered_map<string, double> features;
    
    if (cur_cancel_buffer.empty()) {
        features = {
            {"cancel_buy_num", 0}, {"cancel_sell_num", 0},
            {"cancel_buy_vol", 0}, {"cancel_sell_vol", 0},
            {"cancel_buy_time_range", 0.0}, {"cancel_sell_time_range", 0.0},
            {"cancel_buy_time_med", 0.0}, {"cancel_sell_time_med", 0.0}
        };
    } else {
        int cancel_buy_num = 0, cancel_sell_num = 0;
        long cancel_buy_vol = 0, cancel_sell_vol = 0;
        vector<double> buy_time_list, sell_time_list;
        
        while (!cur_cancel_buffer.empty()) {
            auto cancel = cur_cancel_buffer.front();
            cur_cancel_buffer.pop_front();
            
            long vol = cancel.volume;
            auto time = cancel.timestamp;
            
            try {
                auto match_order = entrust_dict_by_appl_seq_.at(cancel.apply_seq_num);
                auto delta_time = duration_cast<milliseconds>(time - match_order.timestamp).count();
                
                if (cancel.side == 'B') {
                    cancel_buy_num++;
                    cancel_buy_vol += vol;
                    buy_time_list.push_back(delta_time);
                } else if (cancel.side == 'S') {
                    cancel_sell_num++;
                    cancel_sell_vol += vol;
                    sell_time_list.push_back(delta_time);
                }
            } catch (const out_of_range& e) {
                cerr << "Current cancel cannot find matching order: " << cancel.apply_seq_num << endl;
            }
        }
        
        double cancel_buy_time_range = 0.0, cancel_sell_time_range = 0.0;
        double cancel_buy_time_med = 0.0, cancel_sell_time_med = 0.0;
        
        if (!buy_time_list.empty()) {
            auto [min_it, max_it] = minmax_element(buy_time_list.begin(), buy_time_list.end());
            cancel_buy_time_range = *max_it - *min_it;
            cancel_buy_time_med = median(buy_time_list);
        }
        
        if (!sell_time_list.empty()) {
            auto [min_it, max_it] = minmax_element(sell_time_list.begin(), sell_time_list.end());
            cancel_sell_time_range = *max_it - *min_it;
            cancel_sell_time_med = median(sell_time_list);
        }
        
        features = {
            {"cancel_buy_num", static_cast<double>(cancel_buy_num)},
            {"cancel_sell_num", static_cast<double>(cancel_sell_num)},
            {"cancel_buy_vol", static_cast<double>(cancel_buy_vol)},
            {"cancel_sell_vol", static_cast<double>(cancel_sell_vol)},
            {"cancel_buy_time_range", cancel_buy_time_range},
            {"cancel_sell_time_range", cancel_sell_time_range},
            {"cancel_buy_time_med", cancel_buy_time_med},
            {"cancel_sell_time_med", cancel_sell_time_med}
        };
    }
    
    add_feature(feat_.getInstrumentName(iIdx), tradetime, features);
}

double FeatureBuilder::median(vector<double>& v) {
    if (v.empty()) return 0.0;
    sort(v.begin(), v.end());
    size_t n = v.size() / 2;
    if (v.size() % 2 == 0) {
        return (v[n-1] + v[n]) / 2.0;
    } else {
        return v[n];
    }
}
