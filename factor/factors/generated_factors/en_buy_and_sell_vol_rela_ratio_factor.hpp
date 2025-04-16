#ifndef EN_BUY_AND_SELL_VOL_RELA_RATIO_H
#define EN_BUY_AND_SELL_VOL_RELA_RATIO_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class EnBuyAndSellVolRelaRatioFactor : public BaseFactor {
public:
    EnBuyAndSellVolRelaRatioFactor() : BaseFactor("en_buy_and_sell_vol_rela_ratio") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for en_buy_and_sell_vol_rela_ratio
        result_ = 0.0;
    }

private:
int window = 1;
};

REGISTER_FACTOR(EnBuyAndSellVolRelaRatioFactor);

#endif // EN_BUY_AND_SELL_VOL_RELA_RATIO_H
