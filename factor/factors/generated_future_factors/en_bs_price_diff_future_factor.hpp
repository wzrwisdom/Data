#ifndef EN_BS_PRICE_DIFF_FUTURE_H
#define EN_BS_PRICE_DIFF_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class EnBsPriceDiffFutureFactor : public BaseFactor {
public:
    EnBsPriceDiffFutureFactor() : BaseFactor("en_bs_price_diff_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
// No parameters
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for en_bs_price_diff_future
        result_ = 0.0;
    }

private:

};

REGISTER_FACTOR(EnBsPriceDiffFutureFactor);

#endif // EN_BS_PRICE_DIFF_FUTURE_H
