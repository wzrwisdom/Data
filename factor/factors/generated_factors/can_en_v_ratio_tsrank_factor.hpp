#ifndef CAN_EN_V_RATIO_TSRANK_H
#define CAN_EN_V_RATIO_TSRANK_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class CanEnVRatioTsrankFactor : public BaseFactor {
public:
    CanEnVRatioTsrankFactor() : BaseFactor("can_en_v_ratio_tsrank") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window1")) window1 = static_cast<int>(params.at("window1"));
        if (params.count("window2")) window2 = static_cast<int>(params.at("window2"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for can_en_v_ratio_tsrank
        result_ = 0.0;
    }

private:
int window1 = 1;
    int window2 = 1;
};

REGISTER_FACTOR(CanEnVRatioTsrankFactor);

#endif // CAN_EN_V_RATIO_TSRANK_H
