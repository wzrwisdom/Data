#ifndef CLH_DELTA_FUTURE_H
#define CLH_DELTA_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class ClhDeltaFutureFactor : public BaseFactor {
public:
    ClhDeltaFutureFactor() : BaseFactor("clh_delta_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
        if (params.count("shift")) shift = static_cast<int>(params.at("shift"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for clh_delta_future
        result_ = 0.0;
    }

private:
int window = 1;
    int shift = 1;
};

REGISTER_FACTOR(ClhDeltaFutureFactor);

#endif // CLH_DELTA_FUTURE_H
