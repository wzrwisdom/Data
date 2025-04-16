#ifndef OHLC_RAT_H
#define OHLC_RAT_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class OhlcRatFactor : public BaseFactor {
public:
    OhlcRatFactor() : BaseFactor("ohlc_rat") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for ohlc_rat
        result_ = 0.0;
    }

private:
int window = 1;
};

REGISTER_FACTOR(OhlcRatFactor);

#endif // OHLC_RAT_H
