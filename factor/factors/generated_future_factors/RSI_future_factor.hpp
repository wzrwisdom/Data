#ifndef RSI_FUTURE_H
#define RSI_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class RsiFutureFactor : public BaseFactor {
public:
    RsiFutureFactor() : BaseFactor("RSI_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
// No parameters
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for RSI_future
        result_ = 0.0;
    }

private:

};

REGISTER_FACTOR(RsiFutureFactor);

#endif // RSI_FUTURE_H
