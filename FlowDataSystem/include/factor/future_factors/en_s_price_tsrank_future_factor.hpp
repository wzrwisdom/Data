#ifndef EN_S_PRICE_TSRANK_FUTURE_H
#define EN_S_PRICE_TSRANK_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class EnSPriceTsrankFutureFactor : public BaseFactor {
public:
    EnSPriceTsrankFutureFactor() : BaseFactor("en_s_price_tsrank_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto s1 = builder("s1", window);
        auto last = builder("last", window);
        if (s1.size() < std::floor(window/3)) {
            result_ = std::nan("");
            return;
        }
        last = row_round(last, 5);
        s1 = row_round(s1, 4);
        result_ = rank(s1-last, window);
    }

private:
    int window = 1;
};

REGISTER_FACTOR(EnSPriceTsrankFutureFactor);

#endif // EN_S_PRICE_TSRANK_FUTURE_H
