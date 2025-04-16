#ifndef EN_S_RET5_TSRANK_H
#define EN_S_RET5_TSRANK_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class EnSRet5TsrankFactor : public BaseFactor {
public:
    EnSRet5TsrankFactor() : BaseFactor("en_s_ret5_tsrank") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
        if (params.count("shift")) shift = static_cast<int>(params.at("shift"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        int window_tot = window + shift;
        auto ssv5_sum = builder("ssv5_sum", window_tot);
        auto sv5_sum = builder("sv5_sum", window_tot);
        if (sv10_sum.size() < std::floor(window/3) + shift) {
            result_ = std::nan("");
            return;
        }
        result_ = rank(row_ret(ssv10_sum/sv10_sum, shift), window)
    }

private:
    int window = 1;
    int shift = 1;
};

REGISTER_FACTOR(EnSRet5TsrankFactor);

#endif // EN_S_RET5_TSRANK_H
