#ifndef DETREND2_FUTURE_H
#define DETREND2_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class Detrend2FutureFactor : public BaseFactor {
public:
    Detrend2FutureFactor() : BaseFactor("Detrend2_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
// No parameters
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for Detrend2_future
        result_ = 0.0;
    }

private:

};

REGISTER_FACTOR(Detrend2FutureFactor);

#endif // DETREND2_FUTURE_H
