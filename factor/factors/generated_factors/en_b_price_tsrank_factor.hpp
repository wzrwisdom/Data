#ifndef EN_B_PRICE_TSRANK_H
#define EN_B_PRICE_TSRANK_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class EnBPriceTsrankFactor : public BaseFactor {
public:
    EnBPriceTsrankFactor() : BaseFactor("en_b_price_tsrank") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for en_b_price_tsrank
        result_ = 0.0;
    }

private:
int window = 1;
};

REGISTER_FACTOR(EnBPriceTsrankFactor);

#endif // EN_B_PRICE_TSRANK_H
