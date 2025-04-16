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
        // TODO: implement logic for ATR_future
        result_ = 0.0;
    }

private:
int window = 1;
    int timeperiod = 1;
};

REGISTER_FACTOR(AtrFutureFactor);

#endif // ATR_FUTURE_H
