#ifndef PV_CORR_FUTURE_H
#define PV_CORR_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class PvCorrFutureFactor : public BaseFactor {
public:
    PvCorrFutureFactor() : BaseFactor("pv_corr_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto last = builder("last", window);
        auto vol = builder("vol", window);

        if (last.size() < std::floor(window/2)) {
            result_ = std::nan("");
            return;
        }
        result_ = corr(last, vol, window);
        result_ = std::isnan(result_) ? 0 : result_;
    }

private:
    int window = 1;
};

REGISTER_FACTOR(PvCorrFutureFactor);

#endif // PV_CORR_FUTURE_H
