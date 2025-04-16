#ifndef EN_V_ORDER_INBALANCE_H
#define EN_V_ORDER_INBALANCE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class EnVOrderInbalanceFactor : public BaseFactor {
public:
    EnVOrderInbalanceFactor() : BaseFactor("en_v_order_inbalance") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
// No parameters
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for en_v_order_inbalance
        result_ = 0.0;
    }

private:

};

REGISTER_FACTOR(EnVOrderInbalanceFactor);

#endif // EN_V_ORDER_INBALANCE_H
