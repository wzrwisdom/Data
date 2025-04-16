#ifndef TD_P_V_RATIO_FUTURE_H
#define TD_P_V_RATIO_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class TdPVRatioFutureFactor : public BaseFactor {
public:
    TdPVRatioFutureFactor() : BaseFactor("td_p_v_ratio_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window1")) window1 = static_cast<int>(params.at("window1"));
        if (params.count("window2")) window2 = static_cast<int>(params.at("window2"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for td_p_v_ratio_future
        result_ = 0.0;
    }

private:
int window1 = 1;
    int window2 = 1;
};

REGISTER_FACTOR(TdPVRatioFutureFactor);

#endif // TD_P_V_RATIO_FUTURE_H
