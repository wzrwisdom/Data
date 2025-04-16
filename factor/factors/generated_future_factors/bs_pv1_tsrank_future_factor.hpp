#ifndef BS_PV1_TSRANK_FUTURE_H
#define BS_PV1_TSRANK_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class BsPv1TsrankFutureFactor : public BaseFactor {
public:
    BsPv1TsrankFutureFactor() : BaseFactor("bs_pv1_tsrank_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for bs_pv1_tsrank_future
        result_ = 0.0;
    }

private:
int window = 1;
};

REGISTER_FACTOR(BsPv1TsrankFutureFactor);

#endif // BS_PV1_TSRANK_FUTURE_H
