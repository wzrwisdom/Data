#ifndef STOCHRSIFASTD_FUTURE_H
#define STOCHRSIFASTD_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class StochrsifastdFutureFactor : public BaseFactor {
public:
    StochrsifastdFutureFactor() : BaseFactor("STOCHRSIfastd_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        // No parameters
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto last = builder("last", -1);
        result_ = computeSTOCHRSIFASTD(last).back();
    }

private:

};

REGISTER_FACTOR(StochrsifastdFutureFactor);

#endif // STOCHRSIFASTD_FUTURE_H
