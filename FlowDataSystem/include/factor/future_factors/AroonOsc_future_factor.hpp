#ifndef AROONOSC_FUTURE_H
#define AROONOSC_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class AroonoscFutureFactor : public BaseFactor {
public:
    AroonoscFutureFactor() : BaseFactor("AroonOsc_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
        if (params.count("timeperiod")) timeperiod = static_cast<int>(params.at("timeperiod"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        int window_tot = window + timeperiod;
        auto last = builder("last", -1);
        if (last.size() < std::floor(window/3) + timeperiod) {
            result_ = std::nan("");
            return;
        }
        auto minimum = ts_min(last, window);
        auto maximum = ts_max(last, window);
        result_ = computeAROONOSC(maximum, minimum).back();
    }

private:
    int window = 1;
    int timeperiod = 1;
};

REGISTER_FACTOR(AroonoscFutureFactor);

#endif // AROONOSC_FUTURE_H
