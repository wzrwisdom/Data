#ifndef AWESOMEOSC_H
#define AWESOMEOSC_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class AwesomeoscFactor : public BaseFactor {
public:
    AwesomeoscFactor() : BaseFactor("AwesomeOsc") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        int window_tot = window + 34;
        auto low = builder("low", window);
        auto high = builder("high", window);
        if (low.size() < std::floor(window/3) + 34 - 1) {
            result_ = std::nan("");
            return;
        } 
        auto highest = ts_max(high, window);
        auto lowest = ts_min(low, window);
        auto mean = (highest + lowest) / 2;
        auto res = computeSMA(mean, 5) - computeSMA(mean, 34);
        result_ = res.back();

    }

private:
    int window = 1;
};

REGISTER_FACTOR(AwesomeoscFactor);

#endif // AWESOMEOSC_H
