#ifndef PV_CORR_H
#define PV_CORR_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class PvCorrFactor : public BaseFactor {
public:
    PvCorrFactor() : BaseFactor("pv_corr") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto close = builder("close", window);
        auto vol = builder("vol", window);

        if (close.size() < std::floor(window/2)) {
            result_ = 0;
            return;
        }
        result_ = corr(close, vol, window);
        result_ = std::isnan(result_)? 0 : result_;
    }

private:
    int window = 1;
};

REGISTER_FACTOR(PvCorrFactor);

#endif // PV_CORR_H
