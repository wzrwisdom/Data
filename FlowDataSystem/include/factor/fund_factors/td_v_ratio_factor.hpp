#ifndef TD_V_RATIO_H
#define TD_V_RATIO_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class TradeVolRatioFactor : public BaseFactor {
public:
    TradeVolRatioFactor() : BaseFactor("td_v_ratio") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto td_buy_v = builder("td_buy_vol", window);
        auto td_sell_v = builder("td_sell_vol", window);
        if (td_buy_v.size() < std::floor(window/3)) {
            result_ = std::nan("");
            return;
        }
        auto res = row_fillna((td_buy_v - td_sell_v)/(td_buy_v + td_sell_v), 0.0);
        result_ = rank(res, window)
    }

private:
    int window = 1;
};

REGISTER_FACTOR(TradeVolRatioFactor);

#endif // TD_V_RATIO_H
