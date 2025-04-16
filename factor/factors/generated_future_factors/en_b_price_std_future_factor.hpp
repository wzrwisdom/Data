#ifndef EN_B_PRICE_STD_FUTURE_H
#define EN_B_PRICE_STD_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class EnBPriceStdFutureFactor : public BaseFactor {
public:
    EnBPriceStdFutureFactor() : BaseFactor("en_b_price_std_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window1")) window1 = static_cast<int>(params.at("window1"));
        if (params.count("window2")) window2 = static_cast<int>(params.at("window2"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for en_b_price_std_future
        result_ = 0.0;
    }

private:
int window1 = 1;
    int window2 = 1;
};

REGISTER_FACTOR(EnBPriceStdFutureFactor);

#endif // EN_B_PRICE_STD_FUTURE_H
