#ifndef APO_FUTURE_H
#define APO_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class ApoFutureFactor : public BaseFactor {
public:
    ApoFutureFactor() : BaseFactor("APO_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("matype")) matype = static_cast<int>(params.at("matype"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto last = builder("last", -1);
        result_ = computeAPO(last, 5, 13, 1).back();
    }

private:
    int matype = 1;
};

REGISTER_FACTOR(ApoFutureFactor);

#endif // APO_FUTURE_H
