#ifndef EN_BUY_AND_SELL_VOL_RELA_RATIO_H
#define EN_BUY_AND_SELL_VOL_RELA_RATIO_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class EnBuyAndSellVolRelaRatioFactor : public BaseFactor {
public:
    EnBuyAndSellVolRelaRatioFactor() : BaseFactor("en_buy_and_sell_vol_rela_ratio") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto en_buy_v = builder("en_buy_vol", window);
        auto en_sell_v = builder("en_sell_vol", window);
        if (en_buy_v.size() == std::floor(window/3)) {
            result_ = std::nan("");
            return;
        }
        double buy_v = sum(en_buy_v, window);
        double sell_v = sum(en_sell_v, window);
        result_ = (buy_v - sell_v) / (buy_v + sell_v);
    }

private:
    int window = 1;
};

REGISTER_FACTOR(EnBuyAndSellVolRelaRatioFactor);

#endif // EN_BUY_AND_SELL_VOL_RELA_RATIO_H
