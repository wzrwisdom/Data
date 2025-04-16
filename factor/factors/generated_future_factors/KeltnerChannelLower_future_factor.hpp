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
        // TODO: implement logic for KeltnerChannelLower_future
        result_ = 0.0;
    }

private:
int window = 1;
    int timeperiod1 = 1;
    int timeperiod2 = 1;
};

REGISTER_FACTOR(KeltnerchannellowerFutureFactor);

#endif // KELTNERCHANNELLOWER_FUTURE_H
