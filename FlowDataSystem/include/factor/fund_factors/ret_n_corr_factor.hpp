#ifndef RET_N_CORR_H
#define RET_N_CORR_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class RetNCorrFactor : public BaseFactor {
public:
    RetNCorrFactor() : BaseFactor("ret_n_corr") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto close = builder("close", window+1);
        auto td_buy_n = builder("td_buy_num", window);
        auto td_sell_n = builder("td_sell_num", window);

        if (td_buy_n.size() < std::floor(window/2) + 1) {
            result_ = std::nan("");
            return
        }
        int size = td_buy_n.size();
        std::vector<double> ret = ret(close);
        int start_i = ret.size() <= size ? 0 : ret.size() - size;
        ret = std::vector<double>(ret.begin()+start_i, ret.end());
        result_ = ts_corr(ret, td_buy_n, window).back();
    }

private:
    int window = 1;
};

REGISTER_FACTOR(RetNCorrFactor);

#endif // RET_N_CORR_H
