#ifndef WCL_FUTURE_H
#define WCL_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class WclFutureFactor : public BaseFactor {
public:
    WclFutureFactor() : BaseFactor("WCL_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
        if (params.count("shift")) shift = static_cast<int>(params.at("shift"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for WCL_future
        result_ = 0.0;
    }

private:
int window = 1;
    int shift = 1;
};

REGISTER_FACTOR(WclFutureFactor);

#endif // WCL_FUTURE_H
