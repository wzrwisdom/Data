#ifndef TRADE_VOL_IN_HIGH_PRICE_FUTURE_H
#define TRADE_VOL_IN_HIGH_PRICE_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class TradeVolInHighPriceFutureFactor : public BaseFactor {
public:
    TradeVolInHighPriceFutureFactor() : BaseFactor("trade_vol_in_high_price_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
        if (params.count("perc")) perc = static_cast<int>(params.at("perc"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for trade_vol_in_high_price_future
        result_ = 0.0;
    }

private:
int window = 1;
    int perc = 1;
};

REGISTER_FACTOR(TradeVolInHighPriceFutureFactor);

#endif // TRADE_VOL_IN_HIGH_PRICE_FUTURE_H
