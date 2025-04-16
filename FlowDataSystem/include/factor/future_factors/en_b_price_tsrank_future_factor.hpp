#ifndef EN_B_PRICE_TSRANK_FUTURE_H
#define EN_B_PRICE_TSRANK_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class EnBPriceTsrankFutureFactor : public BaseFactor {
public:
    EnBPriceTsrankFutureFactor() : BaseFactor("en_b_price_tsrank_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto b1 = builder("b1", window);
        auto last = builder("last", window);
        if (last.size() < std::floor(window/3)) {
            result_ = std::nan("");
            return;
        }
        result_ = rank(b1-last, window);
    }

private:
    int window = 1;
};

REGISTER_FACTOR(EnBPriceTsrankFutureFactor);

#endif // EN_B_PRICE_TSRANK_FUTURE_H
