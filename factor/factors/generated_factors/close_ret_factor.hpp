#ifndef CLOSE_RET_H
#define CLOSE_RET_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class CloseRetFactor : public BaseFactor {
public:
    CloseRetFactor() : BaseFactor("close_ret") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("shift")) shift = static_cast<int>(params.at("shift"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for close_ret
        result_ = 0.0;
    }

private:
int shift = 1;
};

REGISTER_FACTOR(CloseRetFactor);

#endif // CLOSE_RET_H
