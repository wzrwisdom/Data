#ifndef EN_S_P10_TSRANK_H
#define EN_S_P10_TSRANK_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class EnSP10TsrankFactor : public BaseFactor {
public:
    EnSP10TsrankFactor() : BaseFactor("en_s_p10_tsrank") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for en_s_p10_tsrank
        result_ = 0.0;
    }

private:
int window = 1;
};

REGISTER_FACTOR(EnSP10TsrankFactor);

#endif // EN_S_P10_TSRANK_H
