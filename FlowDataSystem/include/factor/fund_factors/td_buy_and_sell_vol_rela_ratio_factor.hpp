#ifndef TD_BUY_AND_SELL_VOL_RELA_RATIO_H
#define TD_BUY_AND_SELL_VOL_RELA_RATIO_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class TdBuyAndSellVolRelaRatioFactor : public BaseFactor {
public:
    TdBuyAndSellVolRelaRatioFactor() : BaseFactor("td_buy_and_sell_vol_rela_ratio") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto td_buy_v = builder("td_buy_vol", window);
        auto td_sell_v = builder("td_sell_vol", window);
        if (td_buy_v.size() == std::floor(window/3)) {
            result_ = std::nan("");
            return;
        }

        double buy_v = sum(td_buy_v, window);
        double sell_v = sum(td_sell_v, window);
        result_ = (buy_v - sell_v) / (buy_v + sell_v);
    }

private:
    int window = 1;
};

REGISTER_FACTOR(TdBuyAndSellVolRelaRatioFactor);

#endif // TD_BUY_AND_SELL_VOL_RELA_RATIO_H
