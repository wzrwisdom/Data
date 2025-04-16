#ifndef EN_BS_PRICE_DIFF_H
#define EN_BS_PRICE_DIFF_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class EnBsPriceDiffFactor : public BaseFactor {
public:
    EnBsPriceDiffFactor() : BaseFactor("en_bs_price_diff") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        // No parameters
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto en_buy_p = builder("en_buy_price", 1);
        auto en_sell_p = builder("en_sell_price", 1);
        auto avg_p = builder("bs_avg_price", 1);
        auto res = ((en_buy_p - en_sell_p) / avg_p);
        if (res.size() < 1) {
            result_ = std::nan("");
            return;
        }
        result_ = res.back();
    }

private:

};

REGISTER_FACTOR(EnBsPriceDiffFactor);

#endif // EN_BS_PRICE_DIFF_H
