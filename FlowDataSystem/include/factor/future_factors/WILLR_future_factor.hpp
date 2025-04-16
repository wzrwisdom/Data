#ifndef WILLR_FUTURE_H
#define WILLR_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class WillrFutureFactor : public BaseFactor {
public:
    WillrFutureFactor() : BaseFactor("WILLR_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto last = builder("last", -1);

        if (last.size() < std::floor(window/3)) {
            result_ = std::nan("");
            return;
        }

        auto minimum = ts_min(last, window);
        auto maximum = ts_max(last, window);
        auto res = computeWILLR(maximum, minimum, last);
        result_ = res.back();
        
    }

private:
    int window = 1;
};

REGISTER_FACTOR(WillrFutureFactor);

#endif // WILLR_FUTURE_H
