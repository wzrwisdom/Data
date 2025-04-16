#ifndef TD_AND_EN_VOL_RATIO_BUY_DIR_H
#define TD_AND_EN_VOL_RATIO_BUY_DIR_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class TdAndEnVolRatioBuyDirFactor : public BaseFactor {
public:
    TdAndEnVolRatioBuyDirFactor() : BaseFactor("td_and_en_vol_ratio_buy_dir") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for td_and_en_vol_ratio_buy_dir
        result_ = 0.0;
    }

private:
int window = 1;
};

REGISTER_FACTOR(TdAndEnVolRatioBuyDirFactor);

#endif // TD_AND_EN_VOL_RATIO_BUY_DIR_H
