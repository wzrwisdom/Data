#ifndef BEST_V_IMBALANCE_TSRANK_FUTURE_H
#define BEST_V_IMBALANCE_TSRANK_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class BestVImbalanceTsrankFutureFactor : public BaseFactor {
public:
    BestVImbalanceTsrankFutureFactor() : BaseFactor("best_v_imbalance_tsrank_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("shift")) shift = static_cast<int>(params.at("shift"));
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        int window_tot = window + shift;
        auto bv1_sum = builder("bv1_sum", window_tot);
        auto sv1_sum = builder("sv1_sum", window_tot);
        auto b1 = builder("b1", window_tot);
        auto s1 = builder("s1", window_tot);

        if (b1.size() < std::floor(window/3)) {
            result_ = std::nan("");
            return;
        }

        std::vector<bool> b_flag1 = b1 == row_move(b1, shift);
        std::vector<bool> s_flag1 = s1 == row_move(s1, shift);
        std::vector<bool> b_flag2 = b1 < row_move(b1, shift);
        std::vector<bool> s_flag2 = s1 > row_move(b1, shift);

        std::vector<double> bv_change(b_flag1.size(), std::numeric_limits<double>::quiet_NaN());
        std::vector<double> sv_change(b_flag1.size(), std::numeric_limits<double>::quiet_NaN());
        for (size_t i = shift; i < b_flag1.size(); ++i) {
            bv_change[i] = b_flag2[i] ? 0 : (b_flag1[i] ? (bv1_sum[i] - bv1_sum[i - shift]) : bv1_sum[i]);
            sv_change[i] = s_flag2[i] ? 0 : (s_flag1[i] ? (sv1_sum[i] - sv1_sum[i - shift]) : sv1_sum[i]);
        }

        result_ = rank(bv_change - sv_change, window);
    }

private:
    int shift = 1;
    int window = 1;
};

REGISTER_FACTOR(BestVImbalanceTsrankFutureFactor);

#endif // BEST_V_IMBALANCE_TSRANK_FUTURE_H
