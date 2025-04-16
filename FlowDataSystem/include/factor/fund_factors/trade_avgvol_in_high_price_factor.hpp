#ifndef TRADE_AVGVOL_IN_HIGH_PRICE_H
#define TRADE_AVGVOL_IN_HIGH_PRICE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class TradeAvgvolInHighPriceFactor : public BaseFactor {
public:
    TradeAvgvolInHighPriceFactor() : BaseFactor("trade_avgvol_in_high_price") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
        if (params.count("perc")) perc = static_cast<int>(params.at("perc"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        int window_tot = 2 * window - 1;
        auto td_buy_v = builder("td_buy_vol", window);
        auto td_sell_v = builder("td_sell_vol", window);
        auto td_buy_n = builder("td_buy_num", window);
        auto td_sell_n = builder("td_sell_num", window);
        auto price = builder("close", window_tot);
        if (price.size() < window) {
            result_ = 0;
            return;
        }
        auto vol = td_buy_v + td_sell_v;
        auto n = td_buy_n + td_sell_n;
        auto prank = ts_rank(price, window, window);
        prank = std::vector<double>(prank.end() - window, prank.end());
        auto cond = prank > perc + 1e-7;
        double n_in_region = std::accumulate(n.begin(), n.end(), 0., [&cond, i=0](double acc, double value) mutable {
            return cond[i++] ? acc + value : acc;
        } );
        double v_in_region = std::accumulate(vol.begin(), vol.end(), 0., [&cond, i=0](double acc, double value) mutable {
            return cond[i++] ? acc + value : acc;
        } );
        double n_total = std::accumulate(n.begin(), n.end(), 0.);
        double v_total = std::accumulate(vol.begin(), vol.end(), 0.);
        result_ = (v_in_region / n_in_region) / (v_total / n_total);
    }

private:
    int window = 1;
    int perc = 1;
};

REGISTER_FACTOR(TradeAvgvolInHighPriceFactor);

#endif // TRADE_AVGVOL_IN_HIGH_PRICE_H
