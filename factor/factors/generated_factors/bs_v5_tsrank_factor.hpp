#ifndef BS_V5_TSRANK_H
#define BS_V5_TSRANK_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class BsV5TsrankFactor : public BaseFactor {
public:
    BsV5TsrankFactor() : BaseFactor("bs_v5_tsrank") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for bs_v5_tsrank
        result_ = 0.0;
    }

private:
int window = 1;
};

REGISTER_FACTOR(BsV5TsrankFactor);

#endif // BS_V5_TSRANK_H
