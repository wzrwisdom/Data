#ifndef ALLIGATORTEETH_H
#define ALLIGATORTEETH_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class AlligatorteethFactor : public BaseFactor {
public:
    AlligatorteethFactor() : BaseFactor("AlligatorTeeth") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("shift")) shift = static_cast<int>(params.at("shift"));
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        int window_tot = window + shift + 8;
        auto low = builder("low", window);
        auto high = builder("high", window);
        if (low.size() < std::floor(window/3) + shift + 8 - 1) {
            result_ = std::nan("");
            return;
        } 
        auto highest = ts_max(high, window);
        auto lowest = ts_min(low, window);
        auto mean = (highest + lowest) / 2;
        auto res = computeSMA(mean, 8);
        result_ = ret(res, shift);
    }

private:
    int shift = 1;
    int window = 1;
};

REGISTER_FACTOR(AlligatorteethFactor);

#endif // ALLIGATORTEETH_H
