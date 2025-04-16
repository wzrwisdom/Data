#ifndef TRIMA_FUTURE_H
#define TRIMA_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class TrimaFutureFactor : public BaseFactor {
public:
    TrimaFutureFactor() : BaseFactor("TRIMA_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("shift")) shift = static_cast<int>(params.at("shift"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto last = builder("last", -1);
        result_ = computeTRIMA(last, shift).back();
    }

private:
    int shift = 1;
};

REGISTER_FACTOR(TrimaFutureFactor);

#endif // TRIMA_FUTURE_H
