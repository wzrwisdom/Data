#ifndef TD_BUY_RANK_H
#define TD_BUY_RANK_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class TdBuyRankFactor : public BaseFactor {
public:
    TdBuyRankFactor() : BaseFactor("td_buy_rank") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for td_buy_rank
        result_ = 0.0;
    }

private:
int window = 1;
};

REGISTER_FACTOR(TdBuyRankFactor);

#endif // TD_BUY_RANK_H
