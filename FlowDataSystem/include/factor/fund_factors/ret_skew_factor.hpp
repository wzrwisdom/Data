#ifndef RET_SKEW_H
#define RET_SKEW_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class RetSkewFactor : public BaseFactor {
public:
    RetSkewFactor() : BaseFactor("ret_skew") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
        if (params.count("shift")) shift = static_cast<int>(params.at("shift"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        int window_tot = window + shift;
        auto close = builder("close", window_tot);

        if (close.size() < std::floor(window/3)+shift) {
            result_ = std::nan("");
            return;
        }
        auto ret_v = row_ret(row_round(close, 4), shift);
        result_ = skewness(ret_v, window);
    }

private:
    int window = 1;
    int shift = 1;
};

REGISTER_FACTOR(RetSkewFactor);

#endif // RET_SKEW_H
