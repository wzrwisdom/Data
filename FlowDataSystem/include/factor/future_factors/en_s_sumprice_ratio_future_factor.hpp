#ifndef EN_S_SUMPRICE_RATIO_FUTURE_H
#define EN_S_SUMPRICE_RATIO_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class EnSSumpriceRatioFutureFactor : public BaseFactor {
public:
    EnSSumpriceRatioFutureFactor() : BaseFactor("en_s_sumprice_ratio_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto en_sell_p = builder("s1", window);
        auto en_sell_v = builder("sv1_sum", window);
        double avg_p = builder("bs_avg_price", 1).back();

        if (en_sell_p.size() < std::floor(window/3)) {
            result_ = std::nan("");
            return;
        }
        auto sell_avg_p = avg_price(en_sell_p, en_sell_v, window);
        result_ = sell_avg_p.back() / avg_p;
    }

private:
    int window = 1;
};

REGISTER_FACTOR(EnSSumpriceRatioFutureFactor);

#endif // EN_S_SUMPRICE_RATIO_FUTURE_H
