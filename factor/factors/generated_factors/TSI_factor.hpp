#ifndef TSI_H
#define TSI_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class TsiFactor : public BaseFactor {
public:
    TsiFactor() : BaseFactor("TSI") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("shift")) shift = static_cast<int>(params.at("shift"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for TSI
        result_ = 0.0;
    }

private:
int shift = 1;
};

REGISTER_FACTOR(TsiFactor);

#endif // TSI_H
