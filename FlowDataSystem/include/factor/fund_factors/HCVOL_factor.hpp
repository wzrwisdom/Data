#ifndef HCVOL_H
#define HCVOL_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class HcvolFactor : public BaseFactor {
public:
    HcvolFactor() : BaseFactor("HCVOL") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto td_buy_p = builder("td_buy_price", window);
        auto td_buy_v = builder("td_buy_vol", window);
        double close = builder("close", 1).back();

        if (td_buy_p.size() < window) {
            result_ = 0;
            return;
        }
        auto cond = td_buy_p > close + 1e-5;
        std::vector<double> vol(cond.size());


        std::transform(cond.begin(), cond.end(), td_buy_v.begin(), vol.begin(),
                    [](double cond, double val) {
                        return cond ? val : 0;
                    });
        result_ = sum(vol, vol.size()) / sum(td_buy_v, td_buy_v.size());

    }

private:
    int window = 1;
};

REGISTER_FACTOR(HcvolFactor);

#endif // HCVOL_H
