#ifndef OB_PRICE_2DERIVATIVE_FUTURE_H
#define OB_PRICE_2DERIVATIVE_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class ObPrice2derivativeFutureFactor : public BaseFactor {
public:
    ObPrice2derivativeFutureFactor() : BaseFactor("OB_price_2derivative_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("shift")) shift = static_cast<int>(params.at("shift"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for OB_price_2derivative_future
        result_ = 0.0;
    }

private:
int shift = 1;
};

REGISTER_FACTOR(ObPrice2derivativeFutureFactor);

#endif // OB_PRICE_2DERIVATIVE_FUTURE_H
