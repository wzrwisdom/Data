#ifndef ATR_FUTURE_H
#define ATR_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class AtrFutureFactor : public BaseFactor {
public:
    AtrFutureFactor() : BaseFactor("ATR_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
        if (params.count("timeperiod")) timeperiod = static_cast<int>(params.at("timeperiod"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto last = builder("last", -1);
        if (last.size() < timeperiod) {
            result_ = std::nan("");
            return;
        }
        auto maximum = ts_max(last, window);
        auto minimum = ts_min(last, window);
        result_ = computeATR(maximum, minimum, last, timeperiod).back();
    }

private:
    int window = 1;
    int timeperiod = 1;
};

REGISTER_FACTOR(AtrFutureFactor);

#endif // ATR_FUTURE_H
