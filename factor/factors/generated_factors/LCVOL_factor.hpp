#ifndef LCVOL_H
#define LCVOL_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class LcvolFactor : public BaseFactor {
public:
    LcvolFactor() : BaseFactor("LCVOL") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for LCVOL
        result_ = 0.0;
    }

private:
int window = 1;
};

REGISTER_FACTOR(LcvolFactor);

#endif // LCVOL_H
