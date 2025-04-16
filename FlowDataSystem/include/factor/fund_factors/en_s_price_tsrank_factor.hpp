#ifndef EN_S_PRICE_TSRANK_H
#define EN_S_PRICE_TSRANK_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class EnSPriceTsrankFactor : public BaseFactor {
public:
    EnSPriceTsrankFactor() : BaseFactor("en_s_price_tsrank") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto en_sell_price = builder("en_sell_price", window);
        auto avg_p = builder("bs_avg_price", window);
        if (en_s_price.size() < std::floor(window / 3)) {
            result_ = std::nan("");
            return;
        }
        result_ = rank(ffill(row_round(en_sell_price, 5)) - row_round(avg_p, 4), window)
    }

private:
    int window = 1;
};

REGISTER_FACTOR(EnSPriceTsrankFactor);

#endif // EN_S_PRICE_TSRANK_H
