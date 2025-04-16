#ifndef RET_N_CORR_H
#define RET_N_CORR_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class RetNCorrFactor : public BaseFactor {
public:
    RetNCorrFactor() : BaseFactor("ret_n_corr") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for ret_n_corr
        result_ = 0.0;
    }

private:
int window = 1;
};

REGISTER_FACTOR(RetNCorrFactor);

#endif // RET_N_CORR_H
