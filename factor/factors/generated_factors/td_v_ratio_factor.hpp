#ifndef TD_V_RATIO_H
#define TD_V_RATIO_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class TdVRatioFactor : public BaseFactor {
public:
    TdVRatioFactor() : BaseFactor("td_v_ratio") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for td_v_ratio
        result_ = 0.0;
    }

private:
int window = 1;
};

REGISTER_FACTOR(TdVRatioFactor);

#endif // TD_V_RATIO_H
