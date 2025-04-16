#ifndef DPO_FUTURE_H
#define DPO_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class DpoFutureFactor : public BaseFactor {
public:
    DpoFutureFactor() : BaseFactor("DPO_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("timeperiod")) timeperiod = static_cast<int>(params.at("timeperiod"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto last = builder("last", -1);
        if (last.size() < std::floor(timeperiod / 2.0)) {
            result_ = std::nan("");
            return;
        }
        auto res = row_move(last, std::floor(timeperiod/2)+1) - computeSMA(last, timeperiod);
        result_ = res.back();
    }

private:
    int timeperiod = 1;
};

REGISTER_FACTOR(DpoFutureFactor);

#endif // DPO_FUTURE_H
