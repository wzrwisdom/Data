#ifndef CAN_EN_V_RATIO_H
#define CAN_EN_V_RATIO_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class CanEnVRatioFactor : public BaseFactor {
public:
    CanEnVRatioFactor() : BaseFactor("can_en_v_ratio") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
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
        result_ = ((buy_power - sell_power) / (buy_power + sell_power)).back();
        result_ = std::isnan(result_) ? 0 : result_;
    }

private:
    int window = 1;
};

REGISTER_FACTOR(CanEnVRatioFactor);

#endif // CAN_EN_V_RATIO_H
