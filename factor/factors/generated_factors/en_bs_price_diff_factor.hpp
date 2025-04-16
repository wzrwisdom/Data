#ifndef EN_BS_PRICE_DIFF_H
#define EN_BS_PRICE_DIFF_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class EnBsPriceDiffFactor : public BaseFactor {
public:
    EnBsPriceDiffFactor() : BaseFactor("en_bs_price_diff") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
// No parameters
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for en_bs_price_diff
        result_ = 0.0;
    }

private:

};

REGISTER_FACTOR(EnBsPriceDiffFactor);

#endif // EN_BS_PRICE_DIFF_H
