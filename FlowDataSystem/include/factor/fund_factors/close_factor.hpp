#ifndef CLOSE_FACTOR_H
#define CLOSE_FACTOR_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class CloseFactor : public BaseFactor {
public:
    // 构造函数，初始化BaseFactor类，并传入参数"close_ret"
    CloseFactor() : BaseFactor("close_ret") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("shift")) {
            shift = static_cast<int>(params.at("shift"));
        }
    }
    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        std::vector<double> close = builder("close", shift);
        if (close.size() < static_cast<int>(shift + 1)) {
            result_ = std::nan("");
            return
        }
        result_ = close.back() / close[close.size() - shift - 1] - 1;
    }

private:
    int shift = 1;
};

REGISTER_FACTOR(CloseFactor);

#endif // CLOSE_FACTOR_H
