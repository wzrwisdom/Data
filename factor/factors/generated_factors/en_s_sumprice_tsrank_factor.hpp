#ifndef EN_S_SUMPRICE_TSRANK_H
#define EN_S_SUMPRICE_TSRANK_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class EnSSumpriceTsrankFactor : public BaseFactor {
public:
    EnSSumpriceTsrankFactor() : BaseFactor("en_s_sumprice_tsrank") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window1")) window1 = static_cast<int>(params.at("window1"));
        if (params.count("window2")) window2 = static_cast<int>(params.at("window2"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for en_s_sumprice_tsrank
        result_ = 0.0;
    }

private:
int window1 = 1;
    int window2 = 1;
};

REGISTER_FACTOR(EnSSumpriceTsrankFactor);

#endif // EN_S_SUMPRICE_TSRANK_H
