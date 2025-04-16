#ifndef HCP_H
#define HCP_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class HcpFactor : public BaseFactor {
public:
    HcpFactor() : BaseFactor("HCP") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto close = builder("close", 1);
        auto td_buy_p = builder("td_buy_price", window);
        if (close.size() < window) {
            result_ = np.nan("");
            return;
        }
        std::transform(td_buy_p.begin(), td_buy_p.end(), [close.back(), 0](double value) {
            return value > threshold+1e-5 ? value : replace_value;
        });
        result_ = std::accumulate(td_buy_p.begin(), td_buy_p.end(), 0.0)/close.back();
    }

private:
    int window = 1;
};

REGISTER_FACTOR(HcpFactor);

#endif // HCP_H
