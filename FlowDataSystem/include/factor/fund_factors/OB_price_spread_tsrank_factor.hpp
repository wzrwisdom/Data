#ifndef OB_PRICE_SPREAD_TSRANK_H
#define OB_PRICE_SPREAD_TSRANK_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class ObPriceSpreadTsrankFactor : public BaseFactor {
public:
    ObPriceSpreadTsrankFactor() : BaseFactor("OB_price_spread_tsrank") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto s1 = builder("s1", window);
        auto b1 = builder("b1", window);

        if (s1.size() < std::floor(window/3)) {
            result_ = std::nan("");
            return;
        }
        
        std::vector<double> rounded_vec(s1.size());
        std::vector<double> _tmp = s1 - b1;
        std::transform(_tmp.begin(), _tmp.end(), rounded_vec.begin(), [](double x) { return std::round(x*1000)/1000; });
        result_ = rank(rounded_vec, window);
    }

private:
    int window = 1;
};

REGISTER_FACTOR(ObPriceSpreadTsrankFactor);

#endif // OB_PRICE_SPREAD_TSRANK_H
