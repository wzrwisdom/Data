#ifndef MOMENTUM_FUTURE_H
#define MOMENTUM_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class MomentumFutureFactor : public BaseFactor {
public:
    MomentumFutureFactor() : BaseFactor("Momentum_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        // No parameters
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto last = builder("last", -1);
        result_ = computeMomentum(last).back();
    }
private:

};

REGISTER_FACTOR(MomentumFutureFactor);

#endif // MOMENTUM_FUTURE_H
