#ifndef TD_BUY_AND_SELL_VOL_RELA_RATIO_H
#define TD_BUY_AND_SELL_VOL_RELA_RATIO_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class TdBuyAndSellVolRelaRatioFactor : public BaseFactor {
public:
    TdBuyAndSellVolRelaRatioFactor() : BaseFactor("td_buy_and_sell_vol_rela_ratio") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for td_buy_and_sell_vol_rela_ratio
        result_ = 0.0;
    }

private:
int window = 1;
};

REGISTER_FACTOR(TdBuyAndSellVolRelaRatioFactor);

#endif // TD_BUY_AND_SELL_VOL_RELA_RATIO_H
