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
        auto b1 = builder("b1", shift+1);
        if (b1.size() < shift + 1) {
            result_ = std::nan("");
            return;
        }
        result_ = delta(b1, shift);
    }

private:
    int shift = 1;
};

REGISTER_FACTOR(Deripbid1FutureFactor);

#endif // DERIPBID1_FUTURE_H
