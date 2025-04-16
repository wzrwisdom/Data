#ifndef RET_V_PROD_H
#define RET_V_PROD_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class RetVProdFactor : public BaseFactor {
public:
    RetVProdFactor() : BaseFactor("ret_v_prod") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for ret_v_prod
        result_ = 0.0;
    }

private:
int window = 1;
};

REGISTER_FACTOR(RetVProdFactor);

#endif // RET_V_PROD_H
