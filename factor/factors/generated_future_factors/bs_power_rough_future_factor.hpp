#ifndef BS_POWER_ROUGH_FUTURE_H
#define BS_POWER_ROUGH_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class BsPowerRoughFutureFactor : public BaseFactor {
public:
    BsPowerRoughFutureFactor() : BaseFactor("bs_power_rough_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for bs_power_rough_future
        result_ = 0.0;
    }

private:
int window = 1;
};

REGISTER_FACTOR(BsPowerRoughFutureFactor);

#endif // BS_POWER_ROUGH_FUTURE_H
