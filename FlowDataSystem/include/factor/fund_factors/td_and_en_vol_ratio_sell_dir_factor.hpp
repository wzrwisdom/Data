#ifndef TD_AND_EN_VOL_RATIO_SELL_DIR_H
#define TD_AND_EN_VOL_RATIO_SELL_DIR_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class TdAndEnVolRatioSellDirFactor : public BaseFactor {
public:
    TdAndEnVolRatioSellDirFactor() : BaseFactor("td_and_en_vol_ratio_sell_dir") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto td_sell_v = builder("td_sell_vol", window);
        auto en_sell_v = builder("en_sell_vol", window);
        if (td_sell_v.size() < std::floor(window/3)) {
            result_ = 0;
            return;
        }
        result_ = sum(td_sell_v, window) / sum(en_sell_v, window);
        result_ = (std::isnan(result_)) ? 0 : result_;
    }

private:
    int window = 1;
};

REGISTER_FACTOR(TdAndEnVolRatioSellDirFactor);

#endif // TD_AND_EN_VOL_RATIO_SELL_DIR_H
