#ifndef EN_S_PRICE_TSRANK_H
#define EN_S_PRICE_TSRANK_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class EnSPriceTsrankFactor : public BaseFactor {
public:
    EnSPriceTsrankFactor() : BaseFactor("en_s_price_tsrank") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for en_s_price_tsrank
        result_ = 0.0;
    }

private:
int window = 1;
};

REGISTER_FACTOR(EnSPriceTsrankFactor);

#endif // EN_S_PRICE_TSRANK_H
