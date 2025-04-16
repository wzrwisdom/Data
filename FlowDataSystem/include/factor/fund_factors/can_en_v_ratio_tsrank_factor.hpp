#ifndef CAN_EN_V_RATIO_TSRANK_H
#define CAN_EN_V_RATIO_TSRANK_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class CanEnVRatioTsrankFactor : public BaseFactor {
public:
    CanEnVRatioTsrankFactor() : BaseFactor("can_en_v_ratio_tsrank") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window1")) window1 = static_cast<int>(params.at("window1"));
        if (params.count("window2")) window2 = static_cast<int>(params.at("window2"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto en_sell_v = builder("en_sell_vol", window);
        auto en_buy_v = builder("en_buy_vol", window);
        auto can_sell_v = builder("can_sell_vol", window);
        auto can_buy_v = builder("can_buy_vol", window);

        if (can_buy_v.size() < std::floor(window / 3)) {
            result_ = std::nan("");
            return;
        }
        auto buy_power = ts_sum(en_buy_v, window) + ts_sum(can_sell_v, window);
        auto sell_power = ts_sum(en_sell_v, window) + ts_sum(can_buy_v, window);
        auto res = ((buy_power - sell_power) / (buy_power + sell_power));
        result_ = rank(row_fillna(res, 0.), window1);
    }

private:
    int window1 = 1;
    int window2 = 1;
};

REGISTER_FACTOR(CanEnVRatioTsrankFactor);

#endif // CAN_EN_V_RATIO_TSRANK_H
