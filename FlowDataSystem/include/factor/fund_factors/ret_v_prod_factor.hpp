#ifndef RET_VOL_PROD_FACTOR_H
#define RET_VOL_PROD_FACTOR_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class ReturnVolProductFactor : public BaseFactor {
public:
    ReturnVolProductFactor() : BaseFactor("ret_v_prod") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) {
            window = static_cast<int>(params.at("window"));
        }
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        std::vector<double> close = builder("close", window + 1);
        std::vector<double> vol = builder("td_vol", window);

        if (close.size() < std::floor(window / 3) + 1) {
            result_ = std::nan("");
            return;
        }

        std::vector<double> ret_v = row_ret(close, 1);
        ret_v = std::vector<double>(ret_v.begin()+1, ret_v.end());

        std::vector<double> product = ret_v*vol;

        // 排名归一化（这里我们用最后一个值作为排名比例）
        result_ = rank(product, window);
    }

private:
    int window = 1;
};

REGISTER_FACTOR(ReturnVolProductFactor);

#endif // RET_VOL_PROD_FACTOR_H
