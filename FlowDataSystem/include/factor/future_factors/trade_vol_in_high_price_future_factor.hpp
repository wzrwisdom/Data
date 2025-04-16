#ifndef TRADE_VOL_IN_HIGH_PRICE_FUTURE_H
#define TRADE_VOL_IN_HIGH_PRICE_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class TradeVolInHighPriceFutureFactor : public BaseFactor {
public:
    TradeVolInHighPriceFutureFactor() : BaseFactor("trade_vol_in_high_price_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
        if (params.count("perc")) perc = static_cast<int>(params.at("perc"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        int window_tot = 2 * window - 1;
        auto vol = builder("vol", window);
        auto price = builder("last", window_tot);
        if (price.size() < window) {
            result_ = std::nan("");
            return;
        }
        auto prank = ts_rank(price, window, window);
        prank = std::vector<double>(prank.end() - window, prank.end());
        auto cond = prank > perc + 1e-7;
        double v_in_region = std::accumulate(vol.begin(), vol.end(), 0., [&cond, i=0](double acc, double value) mutable {
            return cond[i++] ? acc + value : acc;
        } );
        double v_total = std::accumulate(vol.begin(), vol.end(), 0.);
        result_ = v_in_region / v_total;
    }

private:
    int window = 1;
    int perc = 1;
};

REGISTER_FACTOR(TradeVolInHighPriceFutureFactor);

#endif // TRADE_VOL_IN_HIGH_PRICE_FUTURE_H
