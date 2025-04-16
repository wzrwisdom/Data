#ifndef RET_AVGVOL_CORR_H
#define RET_AVGVOL_CORR_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class RetAvgvolCorrFactor : public BaseFactor {
public:
    RetAvgvolCorrFactor() : BaseFactor("ret_avgvol_corr") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for ret_avgvol_corr
        result_ = 0.0;
    }

private:
int window = 1;
};

REGISTER_FACTOR(RetAvgvolCorrFactor);

#endif // RET_AVGVOL_CORR_H
