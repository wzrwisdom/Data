#ifndef BS_TD_POWER_ROUGH_H
#define BS_TD_POWER_ROUGH_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class BsTdPowerRoughFactor : public BaseFactor {
public:
    BsTdPowerRoughFactor() : BaseFactor("bs_td_power_rough") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto buy_v = builder("td_buy_vol", window);
        auto buy_p = builder("td_buy_price", window);
        auto sell_v = builder("td_sell_vol", window);
        auto sell_p = builder("td_sell_price", window);
        double close = builder("close", 1).back();

        if (buy_v.size() < window) {
            result_ = std::nan("");
            return;
        }
        auto buy_power = buy_v * buy_p / close;
        auto sell_power = (sell_v * (2 *close) - sell_v * sell_p) / close;
        double buy_power_sum = sum(buy_power, window);
        double sell_power_sum = sum(sell_power, window);
        result_ = (buy_power_sum - sell_power_sum) / (buy_power_sum + sell_power_sum + 1e-4);


    }

private:
    int window = 1;
};

REGISTER_FACTOR(BsTdPowerRoughFactor);

#endif // BS_TD_POWER_ROUGH_H
