#ifndef CLH_DELTA_FUTURE_H
#define CLH_DELTA_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class ClhDeltaFutureFactor : public BaseFactor {
public:
    ClhDeltaFutureFactor() : BaseFactor("clh_delta_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
        if (params.count("shift")) shift = static_cast<int>(params.at("shift"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        int window_tot = window + shift;
        auto last = builder("last", window_tot);
        if (last.size() < std::min(static_cast<int>(std::floor(window / 2)), 100)) {
            result_ = std::nan("");
            return;
        }
        int min_periods = std::min(static_cast<int>std::floor(window/3), 100);
        auto c = ts_last(last, window, min_periods);
        auto h = ts_max(last, window, min_periods);
        auto l = ts_min(last, window, min_periods);
        auto res = (h+l-2*c)/ (h - l);
        result_ = delta(res, shift);

    }

private:
    int window = 1;
    int shift = 1;
};

REGISTER_FACTOR(ClhDeltaFutureFactor);

#endif // CLH_DELTA_FUTURE_H
