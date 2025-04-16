#ifndef LCVOL_H
#define LCVOL_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class LcvolFactor : public BaseFactor {
public:
    LcvolFactor() : BaseFactor("LCVOL") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto td_sell_p = builder("td_sell_price", window);
        auto td_sell_v = builder("td_sell_vol", window);
        double close = builder("close", 1).back();

        if (td_sell_p.size() < window) {
            result_ = 0;
            return;
        }
        auto cond = td_sell_p < close - 1e-5;
        std::vector<double> vol(cond.size());


        std::transform(cond.begin(), cond.end(), td_sell_v.begin(), vol.begin(),
                    [](double cond, double val) {
                        return cond ? val : 0;
                    });
        result_ = sum(vol, vol.size()) / sum(td_sell_v, td_sell_v.size());
    }

private:
    int window = 1;
};

REGISTER_FACTOR(LcvolFactor);

#endif // LCVOL_H
