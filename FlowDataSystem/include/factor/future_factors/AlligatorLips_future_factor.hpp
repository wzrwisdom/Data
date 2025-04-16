#ifndef ALLIGATORLIPS_FUTURE_H
#define ALLIGATORLIPS_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class AlligatorlipsFutureFactor : public BaseFactor {
public:
    AlligatorlipsFutureFactor() : BaseFactor("AlligatorLips_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
        if (params.count("shift")) shift = static_cast<int>(params.at("shift"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto last = builder("last", -1);
        auto maximum = ts_max(last, window);
        auto minimum = ts_min(last, window);
        auto mean = (maximum + minimum) / 2;
        auto res = computeSMA(mean, 5);
        result_ = ret(res, shift);
    }

private:
    int window = 1;
    int shift = 1;
};

REGISTER_FACTOR(AlligatorlipsFutureFactor);

#endif // ALLIGATORLIPS_FUTURE_H
