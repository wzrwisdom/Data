#ifndef TD_SELL_RANK_H
#define TD_SELL_RANK_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class TdSellRankFactor : public BaseFactor {
public:
    TdSellRankFactor() : BaseFactor("td_sell_rank") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto td_sell_p = builder("td_sell_price", window);
        auto td_sell_v = builder("td_sell_vol", window);

        if (td_sell_p.size() < std::floor(window/3)) {
            result_ = std::nan("");
            return;
        }
        result_ = rank(td_sell_p * td_sell_v, window)
    }

private:
int window = 1;
};

REGISTER_FACTOR(TdSellRankFactor);

#endif // TD_SELL_RANK_H
