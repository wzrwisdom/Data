#ifndef OB_PRICE_2DERIVATIVE_TSRANK_H
#define OB_PRICE_2DERIVATIVE_TSRANK_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class ObPrice2derivativeTsrankFactor : public BaseFactor {
public:
    ObPrice2derivativeTsrankFactor() : BaseFactor("OB_price_2derivative_tsrank") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
        if (params.count("shift")) shift = static_cast<int>(params.at("shift"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        window_tot = 3 * shift + window;
        auto avg_p = builder("bs_avg_price", window_tot);
        if (avg_p.size() < std::floor(window / 3) + shift) {
            result_ = std::nan("");
            return;
        }

        avg_p = row_round(avg_p, 4);
        auto avg_p_shift = row_move(avg_p, shift);
        auto avg_p_twoshift = row_move(avg_p, shift*2);
        auto res = (avg_p_twoshift + avg_p - avg_p_shift*2) / avg_p;
        result_ = rank(res, window);
    }

private:
    int window = 1;
    int shift = 1;
};

REGISTER_FACTOR(ObPrice2derivativeTsrankFactor);

#endif // OB_PRICE_2DERIVATIVE_TSRANK_H
