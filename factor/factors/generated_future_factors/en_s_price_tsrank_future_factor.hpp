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
        // TODO: implement logic for en_s_price_tsrank_future
        result_ = 0.0;
    }

private:
int window = 1;
};

REGISTER_FACTOR(EnSPriceTsrankFutureFactor);

#endif // EN_S_PRICE_TSRANK_FUTURE_H
