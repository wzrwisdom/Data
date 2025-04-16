#ifndef CAN_EN_V_RATIO_H
#define CAN_EN_V_RATIO_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class CanEnVRatioFactor : public BaseFactor {
public:
    CanEnVRatioFactor() : BaseFactor("can_en_v_ratio") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for can_en_v_ratio
        result_ = 0.0;
    }

private:
int window = 1;
};

REGISTER_FACTOR(CanEnVRatioFactor);

#endif // CAN_EN_V_RATIO_H
