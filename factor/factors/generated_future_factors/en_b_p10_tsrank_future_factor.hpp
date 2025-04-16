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
        // TODO: implement logic for en_b_p10_tsrank_future
        result_ = 0.0;
    }

private:
int window = 1;
};

REGISTER_FACTOR(EnBP10TsrankFutureFactor);

#endif // EN_B_P10_TSRANK_FUTURE_H
