#ifndef EN_B_PRICE_TSRANK_H
#define EN_B_PRICE_TSRANK_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class EnBPriceTsrankFactor : public BaseFactor {
public:
    EnBPriceTsrankFactor() : BaseFactor("en_b_price_tsrank") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto en_buy_p = builder("en_buy_price", window);
        auto avg_p = builder("bs_avg_price", window);
        if (en_buy_p.size() < std::floor(window/3)) {
            result_ = std::nan("");
            return;
        }
        result_ = rank(ffill(en_buy_p) - avg_p, window)
    }

private:
    int window = 1;
};

REGISTER_FACTOR(EnBPriceTsrankFactor);

#endif // EN_B_PRICE_TSRANK_H
