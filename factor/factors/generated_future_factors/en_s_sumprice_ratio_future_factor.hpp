#ifndef EN_S_SUMPRICE_RATIO_FUTURE_H
#define EN_S_SUMPRICE_RATIO_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class EnSSumpriceRatioFutureFactor : public BaseFactor {
public:
    EnSSumpriceRatioFutureFactor() : BaseFactor("en_s_sumprice_ratio_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for en_s_sumprice_ratio_future
        result_ = 0.0;
    }

private:
int window = 1;
};

REGISTER_FACTOR(EnSSumpriceRatioFutureFactor);

#endif // EN_S_SUMPRICE_RATIO_FUTURE_H
