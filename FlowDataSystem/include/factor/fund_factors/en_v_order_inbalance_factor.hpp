#ifndef EN_V_ORDER_INBALANCE_H
#define EN_V_ORDER_INBALANCE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class EnVOrderInbalanceFactor : public BaseFactor {
public:
    EnVOrderInbalanceFactor() : BaseFactor("en_v_order_inbalance") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        // No parameters
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto en_buy_v = builder("en_buy_vol", 1);
        auto en_sell_v = builder("en_sell_vol", 1);
        if (en_buy_v.size()<1) {
            result_ = std::nan("");
            return;
        }
        result_ = (en_buy_v - en_sell_v) / (en_buy_v + en_sell_v);
    }

private:

};

REGISTER_FACTOR(EnVOrderInbalanceFactor);

#endif // EN_V_ORDER_INBALANCE_H
