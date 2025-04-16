#ifndef OB_PRICE_SPREAD_TSRANK_H
#define OB_PRICE_SPREAD_TSRANK_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class ObPriceSpreadTsrankFactor : public BaseFactor {
public:
    ObPriceSpreadTsrankFactor() : BaseFactor("OB_price_spread_tsrank") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for OB_price_spread_tsrank
        result_ = 0.0;
    }

private:
int window = 1;
};

REGISTER_FACTOR(ObPriceSpreadTsrankFactor);

#endif // OB_PRICE_SPREAD_TSRANK_H
