#ifndef MACD_FUTURE_H
#define MACD_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class MacdFutureFactor : public BaseFactor {
public:
    MacdFutureFactor() : BaseFactor("MACD_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        // No parameters
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto last = builder("last", -1);
        result_ = computeMACD(last).back();
    }

private:

};

REGISTER_FACTOR(MacdFutureFactor);

#endif // MACD_FUTURE_H
