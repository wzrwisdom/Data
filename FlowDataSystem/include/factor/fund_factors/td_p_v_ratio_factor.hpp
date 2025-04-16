#ifndef TD_P_V_RATIO_H
#define TD_P_V_RATIO_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class TdPVRatioFactor : public BaseFactor {
public:
    TdPVRatioFactor() : BaseFactor("td_p_v_ratio") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window1")) window1 = static_cast<int>(params.at("window1"));
        if (params.count("window2")) window2 = static_cast<int>(params.at("window2"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        int window = window1 + window2;
        auto vwap = builder("vwap", window);
        auto vol = builder("td_vol", window-1);
        if (vwap.size() < std::floor(window1/3)) {
            result_ = std::nan("");
            return;
        }
        auto delta_vwap = row_delta(vwap, window2);
        int start_index = delta_vwap.size() - window1 - 1;
        delta_vwap = std::vector<double>(delta_vwap.begin() + start_index, delta_vwap.end());
        delta_vwap = row_round(delta_vwap, 10);
        auto vol_sum = ts_sum(vol, window2);
        vol_sum = std::vector<double>(vol_sum.begin() + start_index, vol_sum.end());
        result_ = rank(row_fillna(delta_vwap/vol_sum, 0.0), window1);
    }

private:
    int window1 = 1;
    int window2 = 1;
};

REGISTER_FACTOR(TdPVRatioFactor);

#endif // TD_P_V_RATIO_H
