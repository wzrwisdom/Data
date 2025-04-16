#ifndef TD_AND_EN_VOL_RATIO_BUY_DIR_H
#define TD_AND_EN_VOL_RATIO_BUY_DIR_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class TdAndEnVolRatioBuyDirFactor : public BaseFactor {
public:
    TdAndEnVolRatioBuyDirFactor() : BaseFactor("td_and_en_vol_ratio_buy_dir") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto td_buy_v = builder("td_buy_vol", window);
        auto en_buy_v = builder("en_buy_vol", window);
        if (td_buy_v.size() < std::floor(window/3)) {
            result_ = 0;
            return;
        }
        result_ = sum(td_buy_v, window) / sum(en_buy_v, window);
        result_ = (std::isnan(result_)) ? 0 : result_;
    }


private:
    int window = 1;
};

REGISTER_FACTOR(TdAndEnVolRatioBuyDirFactor);

#endif // TD_AND_EN_VOL_RATIO_BUY_DIR_H
