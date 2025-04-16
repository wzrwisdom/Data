#ifndef WCL_FUTURE_H
#define WCL_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class WclFutureFactor : public BaseFactor {
public:
    WclFutureFactor() : BaseFactor("WCL_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
        if (params.count("shift")) shift = static_cast<int>(params.at("shift"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto last = builder("last", -1);
        if (last.size() < std::floor(window / 3)) {
            result_ = std::nan("");
            return;
        }

        auto minimum = ts_min(last, window);
        auto maximum = ts_max(last, window);
        result_ = computeWCLPRICE(maximum, minimum, last).back();
    }

private:
    int window = 1;
    int shift = 1;
};

REGISTER_FACTOR(WclFutureFactor);

#endif // WCL_FUTURE_H
