#ifndef PV_CORR_FUTURE_H
#define PV_CORR_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class PvCorrFutureFactor : public BaseFactor {
public:
    PvCorrFutureFactor() : BaseFactor("pv_corr_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for pv_corr_future
        result_ = 0.0;
    }

private:
int window = 1;
};

REGISTER_FACTOR(PvCorrFutureFactor);

#endif // PV_CORR_FUTURE_H
