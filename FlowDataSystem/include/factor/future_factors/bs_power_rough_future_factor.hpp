#ifndef BS_POWER_ROUGH_FUTURE_H
#define BS_POWER_ROUGH_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class BsPowerRoughFutureFactor : public BaseFactor {
public:
    BsPowerRoughFutureFactor() : BaseFactor("bs_power_rough_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto buy_v = builder("bv1_sum", window);
        auto buy_p = builder("b1", window);
        auto sell_v = builder("sv1_sum", window);
        auto sell_p = builder("s1", window);
        double close = builder("last", 1).back();

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

REGISTER_FACTOR(BsPowerRoughFutureFactor);

#endif // BS_POWER_ROUGH_FUTURE_H
