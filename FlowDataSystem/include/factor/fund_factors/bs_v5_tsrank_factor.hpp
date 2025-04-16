#ifndef BS_V5_TSRANK_H
#define BS_V5_TSRANK_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"


class BsV5TsrankFactor : public BaseFactor {
public:
    BsV5TsrankFactor() : BaseFactor("bs_v5_tsrank") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto bv5_sum = builder("bv5_sum", window);
        auto sv5_sum = builder("sv5_sum", window);
        if (sv5_sum.size() < std::floor(window/3)) {
            result_ = std::nan("");
            return;
        }

        result_ = rank(bv5_sum - sv5_sum, window);
    }

private:
    int window = 1;
};

REGISTER_FACTOR(BsV5TsrankFactor);

#endif // BS_V5_TSRANK_H
