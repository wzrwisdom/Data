#ifndef EN_S_P10_TSRANK_H
#define EN_S_P10_TSRANK_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class EnSP10TsrankFactor : public BaseFactor {
public:
    EnSP10TsrankFactor() : BaseFactor("en_s_p10_tsrank") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto ssv10_sum = builder("ssv10_sum", window);
        auto sv10_sum = builder("sv10_sum", window);
        if (sv10_sum.size() < std::floor(window/3)) {
            result_ = std::nan("");
            return;
        }
        result_ = rank(ssv10_sum/sv10_sum, window)
    }

private:
    int window = 1;
};

REGISTER_FACTOR(EnSP10TsrankFactor);

#endif // EN_S_P10_TSRANK_H
