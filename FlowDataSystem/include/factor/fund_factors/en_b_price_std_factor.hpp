#ifndef EN_B_PRICE_STD_H
#define EN_B_PRICE_STD_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class EnBPriceStdFactor : public BaseFactor {
public:
    EnBPriceStdFactor() : BaseFactor("en_b_price_std") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window1")) window1 = static_cast<int>(params.at("window1"));
        if (params.count("window2")) window2 = static_cast<int>(params.at("window2"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        int window_tot = window1 + window2 - 1;
        auto en_buy_p = builder("en_buy_price", window_tot);
        if (en_buy_p.size() < std::floor(window_tot/3)-1) {
            result_ = std::nan("");
            return;
        }
        auto std_values = ts_std(en_buy_p, window2);
        result_ = rank(std_values, window1);
    }

private:
    int window1 = 1;
    int window2 = 1;
};

REGISTER_FACTOR(EnBPriceStdFactor);

#endif // EN_B_PRICE_STD_H
