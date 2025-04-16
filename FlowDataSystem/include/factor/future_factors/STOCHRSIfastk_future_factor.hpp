#ifndef STOCHRSIFASTK_FUTURE_H
#define STOCHRSIFASTK_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class StochrsifastkFutureFactor : public BaseFactor {
public:
    StochrsifastkFutureFactor() : BaseFactor("STOCHRSIfastk_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
// No parameters
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto last = builder("last", -1);
        result_ = computeSTOCHRSIFASTK(last).back();
    }

private:

};

REGISTER_FACTOR(StochrsifastkFutureFactor);

#endif // STOCHRSIFASTK_FUTURE_H
