#ifndef TRADE_NUM_IN_LOW_PRICE_H
#define TRADE_NUM_IN_LOW_PRICE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class TradeNumInLowPriceFactor : public BaseFactor {
public:
    TradeNumInLowPriceFactor() : BaseFactor("trade_num_in_low_price") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
        if (params.count("perc")) perc = static_cast<int>(params.at("perc"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for trade_num_in_low_price
        result_ = 0.0;
    }

private:
int window = 1;
    int perc = 1;
};

REGISTER_FACTOR(TradeNumInLowPriceFactor);

#endif // TRADE_NUM_IN_LOW_PRICE_H
