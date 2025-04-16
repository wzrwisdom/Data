#ifndef OHLC_RAT_H
#define OHLC_RAT_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class OhlcRatFactor : public BaseFactor {
public:
    OhlcRatFactor() : BaseFactor("ohlc_rat") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto open = builder("open", window);
        auto high = builder("high", window);
        auto low = builder("low", window);
        auto close = builder("close", window);

        if (open.size() < std::min(100, std::floor(window/2))) {
            result_ = std::nan("");
            return;
        }
        double o = open.front();
        double c = close.back();
        double h = *std::max_element(high.begin(), high.end());
        double l = *std::min_element(low.begin(), low.end());
        result_  = (c - o) / (h - l);
    }

private:
    int window = 1;
};

REGISTER_FACTOR(OhlcRatFactor);

#endif // OHLC_RAT_H
