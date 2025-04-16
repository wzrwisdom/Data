#ifndef TSI_H
#define TSI_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class TSIFactor : public BaseFactor {
public:
    TSIFactor() : BaseFactor("TSI") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("shift")) shift = static_cast<int>(params.at("shift"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        timeperiod1 = shift * 25;
        timeperiod2 = shift * 13
        int window_tot = timeperiod1 + timeperiod2 + window;
        auto close = builder("close", -1);
        if (close.size() < window_tot) {
            result_ = std::nan("");
            return;
        }
        auto det_c = row_delta(close, shift);

        auto ema1 = computeEMA(det_c, timeperiod1);
        auto ema2 = computeEMA(ema1, timeperiod1);
        auto res = computeEMA(ema1, timeperiod2) * 100 / computeEMA(ema2, timeperiod2);
        result_ = res.back();
    }

private:
    int shift = 1;
};

REGISTER_FACTOR(TSIFactor);

#endif // TSI_H
