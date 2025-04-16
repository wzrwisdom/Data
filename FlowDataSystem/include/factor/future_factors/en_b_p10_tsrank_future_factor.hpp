#ifndef EN_B_P10_TSRANK_FUTURE_H
#define EN_B_P10_TSRANK_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class EnBP10TsrankFutureFactor : public BaseFactor {
public:
    EnBP10TsrankFutureFactor() : BaseFactor("en_b_p10_tsrank_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto b1 = builder("b1", window);
        if (b1.size() < std::floor(window/3)) {
            result_ = std::nan("");
            return;
        }
        result_ = rank(b1, window);
    }

private:
    int window = 1;
};

REGISTER_FACTOR(EnBP10TsrankFutureFactor);

#endif // EN_B_P10_TSRANK_FUTURE_H
