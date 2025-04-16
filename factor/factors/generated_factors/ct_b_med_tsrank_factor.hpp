#ifndef CT_B_MED_TSRANK_H
#define CT_B_MED_TSRANK_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class CtBMedTsrankFactor : public BaseFactor {
public:
    CtBMedTsrankFactor() : BaseFactor("ct_b_med_tsrank") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for ct_b_med_tsrank
        result_ = 0.0;
    }

private:
int window = 1;
};

REGISTER_FACTOR(CtBMedTsrankFactor);

#endif // CT_B_MED_TSRANK_H
