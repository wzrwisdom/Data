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
        auto b1 = builder("b1", 1);
        auto s1 = builder("s1", 1);
        auto avg_p = builder("bs_avg_price", 1);
        auto res = ((b1 - s1) / avg_p);
        if (res.size() < 1) {
            result_ = std::nan("");
            return;
        }
        result_ = res.back();
    }

private:

};

REGISTER_FACTOR(EnBsPriceDiffFutureFactor);

#endif // EN_BS_PRICE_DIFF_FUTURE_H
