#ifndef MARKET_DATA_TYPES_HPP
#define MARKET_DATA_TYPES_HPP

#include <chrono>
#include <string>
#include <core/Utils.hpp>

namespace MarketData {

struct FutureSnapData {
    double total_turnover;
    double vol;
    double last; 
    double s1;
    double b1; 
    double sv1_sum; 
    double bv1_sum;
    double bs_avg_price;
    std::string symbol;
    Utils::Timestamp timestamp;
};

struct SnapData {
    double last_price;
    double bid_prices[10];
    double ask_prices[10];
    double bid_volumes[10];
    double ask_volumes[10];
    std::string symbol;
    Utils::Timestamp timestamp;
};

struct TradeData {
    double price;
    double volume;
    char side; // 'B' for buy, 'S' for sell
    char exec_type;
    long apply_seq_num;
    long bid_apply_seq_num;
    long ask_apply_seq_num;
    std::string symbol;
    Utils::Timestamp timestamp;
};

struct OrderData {
    double price;
    double volume;
    char side; // 'B' for buy, 'S' for sell
    long apply_seq_num; 
    long orig_order_no; // original order ID for cancel/modify
    char order_type; 
    long biz_index;
    std::string symbol;
    Utils::Timestamp timestamp;
};

struct CancelData {
    long apply_seq_num;
    char side;
    double volume;
    std::string symbol;
    Utils::Timestamp timestamp;
};

} // namespace MarketData

#endif // MARKET_DATA_TYPES_HPP