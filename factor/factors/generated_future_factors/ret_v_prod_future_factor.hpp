#ifndef RET_V_PROD_FUTURE_H
#define RET_V_PROD_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class RetVProdFutureFactor : public BaseFactor {
public:
    RetVProdFutureFactor() : BaseFactor("ret_v_prod_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for ret_v_prod_future
        result_ = 0.0;
    }

private:
int window = 1;
};

REGISTER_FACTOR(RetVProdFutureFactor);

#endif // RET_V_PROD_FUTURE_H
