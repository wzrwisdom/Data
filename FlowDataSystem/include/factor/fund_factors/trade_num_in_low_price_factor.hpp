#ifndef TRADE_NUM_IN_LOW_PRICE_H
#define TRADE_NUM_IN_LOW_PRICE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class TradeNumInLowPriceFactor : public BaseFactor {
public:
    TradeNumInLowPriceFactor() : BaseFactor("trade_num_in_low_price") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
        if (params.count("perc")) perc = static_cast<int>(params.at("perc"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        int window_tot = 2 * window - 1;
        auto td_buy_n = builder("td_buy_num", window);
        auto td_sell_n = builder("td_sell_num", window);
        auto close = builder("close", window_tot);
        if (close.size() < window) {
            result_ = std::nan("");
            return;
        }
        auto num = td_buy_n + td_sell_n;
        auto prank = ts_rank(close, window, window);
        prank = std::vector<double>(prank.end() - window, prank.end());
        auto cond = prank > perc;
        double n_in_region = std::accumulate(num.begin(), num.end(), 0., [&cond, i=0](double acc, double value) mutable {
            return cond[i++] ? acc + value : acc;
        } );
        double n_total = std::accumulate(num.begin(), num.end(), 0.);
        result_ = n_in_region / n_total;
    }

private:
    int window = 1;
    int perc = 1;
};

REGISTER_FACTOR(TradeNumInLowPriceFactor);

#endif // TRADE_NUM_IN_LOW_PRICE_H
