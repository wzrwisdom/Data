#ifndef EN_SELL_SUM_PRICE_FACTOR_H
#define EN_SELL_SUM_PRICE_FACTOR_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class EnSellSumPriceFactor : public BaseFactor {
public:
    EnSellSumPriceFactor() : BaseFactor("en_s_sumprice_tsrank") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window1")) {
            window1 = static_cast<int>(params.at("window1"));
        }
        if (params.count("window2")) {
            window2 = static_cast<int>(params.at("window2"));
        }
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        int total_window = window1 + window2 - 1;
        auto en_sell_p = builder("en_sell_price", total_window);
        auto en_sell_v = builder("en_sell_vol", total_window);
        auto avg_p = builder("bs_avg_price", window1);

        if (en_sell_p.size() < std::floor((window1 + window2) / 3) - 1) {
            result_ = std::nan("");
            return;
        }   
        auto _tmp = avg_price(ffill(en_sell_p), en_sell_v, window2);
        int start_i = avg_price.size() >window1? avg_price.size() - window1 : 0;
        std::vector<double> sell_avg_p(_tmp.begin()+start_i, _tmp.end());
        result_ = rank(sell_avg_p - avg_p, window1)
    }

private:
    int window1 = 1;
    int window2 = 1;
};

REGISTER_FACTOR(EnSellSumPriceFactor);

#endif // EN_SELL_SUM_PRICE_FACTOR_H
