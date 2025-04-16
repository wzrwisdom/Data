#ifndef BEST_V_IMBALANCE_TSRANK_FUTURE_H
#define BEST_V_IMBALANCE_TSRANK_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class BestVImbalanceTsrankFutureFactor : public BaseFactor {
public:
    BestVImbalanceTsrankFutureFactor() : BaseFactor("best_v_imbalance_tsrank_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("shift")) shift = static_cast<int>(params.at("shift"));
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for best_v_imbalance_tsrank_future
        result_ = 0.0;
    }

private:
int shift = 1;
    int window = 1;
};

REGISTER_FACTOR(BestVImbalanceTsrankFutureFactor);

#endif // BEST_V_IMBALANCE_TSRANK_FUTURE_H
