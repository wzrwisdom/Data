#ifndef EN_B_RET10_TSRANK_H
#define EN_B_RET10_TSRANK_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class EnBRet10TsrankFactor : public BaseFactor {
public:
    EnBRet10TsrankFactor() : BaseFactor("en_b_ret10_tsrank") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
        if (params.count("shift")) shift = static_cast<int>(params.at("shift"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for en_b_ret10_tsrank
        result_ = 0.0;
    }

private:
int window = 1;
    int shift = 1;
};

REGISTER_FACTOR(EnBRet10TsrankFactor);

#endif // EN_B_RET10_TSRANK_H
