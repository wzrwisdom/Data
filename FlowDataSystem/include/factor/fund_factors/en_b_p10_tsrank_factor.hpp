#ifndef EN_B_P10_TSRANK_H
#define EN_B_P10_TSRANK_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class EnBP10TsrankFactor : public BaseFactor {
public:
    EnBP10TsrankFactor() : BaseFactor("en_b_p10_tsrank") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto bbv10_sum = builder("bbv10_sum", window);
        auto bv10_sum = builder("bv10_sum", window);
        if (bv10_sum.size() < std::floor(window/3)) {
            result_ = std::nan("");
            return;
        }
        result_ = rank(bbv10_sum/bv10_sum, window)
    }

private:
    int window = 1;
};

REGISTER_FACTOR(EnBP10TsrankFactor);

#endif // EN_B_P10_TSRANK_H
