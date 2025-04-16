#ifndef AROONDOWN_H
#define AROONDOWN_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class AroondownFactor : public BaseFactor {
public:
    AroondownFactor() : BaseFactor("AroonDown") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
        if (params.count("timeperiod")) timeperiod = static_cast<int>(params.at("timeperiod"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for AroonDown
        result_ = 0.0;
    }

private:
int window = 1;
    int timeperiod = 1;
};

REGISTER_FACTOR(AroondownFactor);

#endif // AROONDOWN_H
