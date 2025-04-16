#ifndef AROONDOWN_H
#define AROONDOWN_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class AroondownFactor : public BaseFactor {
public:
    AroondownFactor() : BaseFactor("AroonDown") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
        if (params.count("timeperiod")) timeperiod = static_cast<int>(params.at("timeperiod"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        int window_tot = window + timeperiod;
        auto low = builder("low", -1);
        auto high = builder("high", -1);
        if (low.size() < std::floor(window/3) + timeperiod) {
            result_ = std::nan("");
            return;
        }
        auto lowest = ts_min(low, window);
        auto highest = ts_max(high, window);
        auto res = computeAROONDown(high, low, timeperiod);
        result_ = res.back();
    }

private:
int window = 1;
    int timeperiod = 1;
};

REGISTER_FACTOR(AroondownFactor);

#endif // AROONDOWN_H
