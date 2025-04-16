#ifndef TD_RET_V_PROD_FUTURE_H
#define TD_RET_V_PROD_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class TdRetVProdFutureFactor : public BaseFactor {
public:
    TdRetVProdFutureFactor() : BaseFactor("td_ret_v_prod_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for td_ret_v_prod_future
        result_ = 0.0;
    }

private:
int window = 1;
};

REGISTER_FACTOR(TdRetVProdFutureFactor);

#endif // TD_RET_V_PROD_FUTURE_H
