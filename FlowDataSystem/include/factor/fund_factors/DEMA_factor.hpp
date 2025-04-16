#ifndef DEMA_H
#define DEMA_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class DemaFactor : public BaseFactor {
public:
    DemaFactor() : BaseFactor("DEMA") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("timeperiod")) timeperiod = static_cast<int>(params.at("timeperiod"));
        if (params.count("shift")) shift = static_cast<int>(params.at("shift"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        int required_window = shift + 2 * timeperiod - 1;
        auto close = builder("close", -1);
        if (close.size() < required_window) {
            result_ = std::nan("");
            return;
        }
        
        auto dema = computeDEMA(close, timeperiod);
        result_ = dema.back()/dema[close.size()-shift-1] -1;

    }

private:
    int timeperiod = 1;
    int shift = 1;
};

REGISTER_FACTOR(DemaFactor);

#endif // DEMA_H
