#ifndef ALLIGATORTEETH_H
#define ALLIGATORTEETH_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class AlligatorteethFactor : public BaseFactor {
public:
    AlligatorteethFactor() : BaseFactor("AlligatorTeeth") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("shift")) shift = static_cast<int>(params.at("shift"));
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for AlligatorTeeth
        result_ = 0.0;
    }

private:
int shift = 1;
    int window = 1;
};

REGISTER_FACTOR(AlligatorteethFactor);

#endif // ALLIGATORTEETH_H
