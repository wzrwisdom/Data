#ifndef CT_B_MED_TSRANK_H
#define CT_B_MED_TSRANK_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class CtBMedTsrankFactor : public BaseFactor {
public:
    CtBMedTsrankFactor() : BaseFactor("ct_b_med_tsrank") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto ct_b_med = builder("cancel_buy_time_med", window);
        if (ct_b_med.size() < std::floor(window/3)) {
            result_ = std::nan("");
            return;
        }
        auto inv_t = 1 / ct_b_med;
        result_ = rank(inv_t, window);
    }

private:
    int window = 1;
};

REGISTER_FACTOR(CtBMedTsrankFactor);

#endif // CT_B_MED_TSRANK_H
