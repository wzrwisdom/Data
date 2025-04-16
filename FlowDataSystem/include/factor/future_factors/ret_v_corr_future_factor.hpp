#ifndef RET_V_CORR_FUTURE_H
#define RET_V_CORR_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class RetVCorrFutureFactor : public BaseFactor {
public:
    RetVCorrFutureFactor() : BaseFactor("ret_v_corr_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto last = builder("last", window+1);
        auto vol = builder("vol", window);
        if (last.size() < std::floor(window/2) + 1) {
            result_ = std::nan("");
            return;
        }
        int size = vol.size();
        auto ret_v = row_ret(last);
        ret_v = std::vector<double>(ret_v.end()-size, ret_v.end());
        result_ = corr(ret_v, vol, window);
    }

private:
    int window = 1;
};

REGISTER_FACTOR(RetVCorrFutureFactor);

#endif // RET_V_CORR_FUTURE_H
