#ifndef HCVOL_H
#define HCVOL_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class HcvolFactor : public BaseFactor {
public:
    HcvolFactor() : BaseFactor("HCVOL") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for HCVOL
        result_ = 0.0;
    }

private:
int window = 1;
};

REGISTER_FACTOR(HcvolFactor);

#endif // HCVOL_H
