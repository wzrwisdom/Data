#ifndef BS_TD_POWER_ROUGH_H
#define BS_TD_POWER_ROUGH_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class BsTdPowerRoughFactor : public BaseFactor {
public:
    BsTdPowerRoughFactor() : BaseFactor("bs_td_power_rough") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for bs_td_power_rough
        result_ = 0.0;
    }

private:
int window = 1;
};

REGISTER_FACTOR(BsTdPowerRoughFactor);

#endif // BS_TD_POWER_ROUGH_H
