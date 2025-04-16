#ifndef DERIPBID1_FUTURE_H
#define DERIPBID1_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class Deripbid1FutureFactor : public BaseFactor {
public:
    Deripbid1FutureFactor() : BaseFactor("DeriPBid1_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("shift")) shift = static_cast<int>(params.at("shift"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for DeriPBid1_future
        result_ = 0.0;
    }

private:
int shift = 1;
};

REGISTER_FACTOR(Deripbid1FutureFactor);

#endif // DERIPBID1_FUTURE_H
