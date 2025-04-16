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
        int window_tot = 2 * shift + 1;
        auto avg_p = builder("bs_avg_price", window_tot);
        if (avg_p.size() < window_tot) {
            result_ = std::nan("");
            return;
        }
        avg_p = row_round(avg_p, 4);
        auto avg_p_shift = row_move(avg_p, shift);
        auto avg_p_twoshift = row_move(avg_p, shift*2);
        auto res = (avg_p_twoshift + avg_p - avg_p_shift*2) / avg_p;
        result_ = res.back();
    }

private:
    int shift = 1;
};

REGISTER_FACTOR(ObPrice2derivativeFutureFactor);

#endif // OB_PRICE_2DERIVATIVE_FUTURE_H
