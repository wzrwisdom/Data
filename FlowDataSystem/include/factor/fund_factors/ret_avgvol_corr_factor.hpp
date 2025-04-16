#ifndef RET_AVGVOL_CORR_H
#define RET_AVGVOL_CORR_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class RetAvgvolCorrFactor : public BaseFactor {
public:
    RetAvgvolCorrFactor() : BaseFactor("ret_avgvol_corr") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto close = builder("close", window+1);
        auto vol = builder("vol", window);
        auto td_buy_num = builder("td_buy_num", window);
        auto td_sell_num = builder("td_sell_num", window);

        if (close.size() < std::floor(window/2) + 1) {
            result_ = 0;
            return;
        }
        auto ret_v = row_ret(close, 1);
        ret_v = std::vector<double>(ret_v.end()-size, ret_v.end());

        result_ = corr(ret_v, row_fillna(vol/(td_buy_num + td_sell_num), 0), window);
        result_ = std::isnan(result_)? 0 : result_;
    }

private:
    int window = 1;
};

REGISTER_FACTOR(RetAvgvolCorrFactor);

#endif // RET_AVGVOL_CORR_H
