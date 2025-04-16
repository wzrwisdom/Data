#ifndef OB_PRICE_2DERIVATIVE_TSRANK_H
#define OB_PRICE_2DERIVATIVE_TSRANK_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class ObPrice2derivativeTsrankFactor : public BaseFactor {
public:
    ObPrice2derivativeTsrankFactor() : BaseFactor("OB_price_2derivative_tsrank") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
        if (params.count("shift")) shift = static_cast<int>(params.at("shift"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for OB_price_2derivative_tsrank
        result_ = 0.0;
    }

private:
int window = 1;
    int shift = 1;
};

REGISTER_FACTOR(ObPrice2derivativeTsrankFactor);

#endif // OB_PRICE_2DERIVATIVE_TSRANK_H
