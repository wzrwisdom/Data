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
        // TODO: implement logic for Momentum_future
        result_ = 0.0;
    }

private:

};

REGISTER_FACTOR(MomentumFutureFactor);

#endif // MOMENTUM_FUTURE_H
