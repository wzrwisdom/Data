#ifndef TD_PRICE_STD_H
#define TD_PRICE_STD_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class TdPriceStdFactor : public BaseFactor {
public:
    TdPriceStdFactor() : BaseFactor("td_price_std") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window1")) window1 = static_cast<int>(params.at("window1"));
        if (params.count("window2")) window2 = static_cast<int>(params.at("window2"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        int window_tot = window1 + window2;
        auto vwap = builder("vwap", window_tot);
        if (vwap.size() < std::floor(window_tot/3) - 1) {
            result_ = std::nan("");
            return;
        }
        auto std_values = ts_std(vwap, window2);
        result_ = rank(std_values, window1);
    }

private:
    int window1 = 1;
    int window2 = 1;
};

REGISTER_FACTOR(TdPriceStdFactor);

#endif // TD_PRICE_STD_H
