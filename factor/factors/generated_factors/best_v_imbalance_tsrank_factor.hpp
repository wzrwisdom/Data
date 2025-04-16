#ifndef BEST_V_IMBALANCE_TSRANK_H
#define BEST_V_IMBALANCE_TSRANK_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class BestVImbalanceTsrankFactor : public BaseFactor {
public:
    BestVImbalanceTsrankFactor() : BaseFactor("best_v_imbalance_tsrank") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
        if (params.count("shift")) shift = static_cast<int>(params.at("shift"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for best_v_imbalance_tsrank
        result_ = 0.0;
    }

private:
int window = 1;
    int shift = 1;
};

REGISTER_FACTOR(BestVImbalanceTsrankFactor);

#endif // BEST_V_IMBALANCE_TSRANK_H
