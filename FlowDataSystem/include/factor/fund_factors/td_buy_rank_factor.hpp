#ifndef TD_BUY_RANK_H
#define TD_BUY_RANK_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class TdBuyRankFactor : public BaseFactor {
public:
    TdBuyRankFactor() : BaseFactor("td_buy_rank") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto td_buy_p = builder("td_buy_price", window);
        auto td_buy_v = builder("td_buy_vol", window);

        if (td_buy_p.size() < std::floor(window/3)) {
            result_ = std::nan("");
            return;
        }
        result_ = rank(td_buy_p * td_buy_v, window)
    }

private:
    int window = 1;
};

REGISTER_FACTOR(TdBuyRankFactor);

#endif // TD_BUY_RANK_H
