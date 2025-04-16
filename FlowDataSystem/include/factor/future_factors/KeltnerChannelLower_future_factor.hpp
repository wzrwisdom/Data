#ifndef KELTNERCHANNELLOWER_FUTURE_H
#define KELTNERCHANNELLOWER_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class KeltnerchannellowerFutureFactor : public BaseFactor {
public:
    KeltnerchannellowerFutureFactor() : BaseFactor("KeltnerChannelLower_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
        if (params.count("timeperiod1")) timeperiod1 = static_cast<int>(params.at("timeperiod1"));
        if (params.count("timeperiod2")) timeperiod2 = static_cast<int>(params.at("timeperiod2"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto last = builder("last", -1);
        if (last.size() < timeperiod1) {
            result_ = std::nan("");
            return;
        }
        auto maximum = ts_max(last, window);
        auto minimum = ts_min(last, window);
        auto atr = computeATR(maximum, minimum, last, timeperiod1);
        auto ema = computeEMA(last, timeperiod2);
        auto res = ((ema - atr*2) - last)/ema;
        result_ = res.back();
    }

private:
    int window = 1;
    int timeperiod1 = 1;
    int timeperiod2 = 1;
};

REGISTER_FACTOR(KeltnerchannellowerFutureFactor);

#endif // KELTNERCHANNELLOWER_FUTURE_H
