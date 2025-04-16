#ifndef EN_B_SUMPRICE_TSRANK_H
#define EN_B_SUMPRICE_TSRANK_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class EnBSumpriceTsrankFactor : public BaseFactor {
public:
    EnBSumpriceTsrankFactor() : BaseFactor("en_b_sumprice_tsrank") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window1")) {
            window1 = static_cast<int>(params.at("window1"));
        }
        if (params.count("window2")) {
            window2 = static_cast<int>(params.at("window2"));
        }
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        int total_window = window1 + window2 - 1;
        auto en_buy_p = builder("en_buy_price", total_window);
        auto en_buy_v = builder("en_buy_vol", total_window);
        auto avg_p = builder("bs_avg_price", window1);

        if (en_buy_p.size() < std::floor((window1 + window2) / 3) - 1) {
            result_ = std::nan("");
            return;
        }   
        auto _tmp = avg_price(ffill(en_buy_p), en_buy_v, window2);
        int start_i = avg_price.size() >window1? avg_price.size() - window1 : 0;
        std::vector<double> buy_avg_p(_tmp.begin()+start_i, _tmp.end());
        result_ = rank(buy_avg_p - avg_p, window1)
    }

private:
    int window1 = 1;
    int window2 = 1;
};

REGISTER_FACTOR(EnBSumpriceTsrankFactor);

#endif // EN_B_SUMPRICE_TSRANK_H
