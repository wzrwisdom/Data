#ifndef AWESOMEOSC_H
#define AWESOMEOSC_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class AwesomeoscFactor : public BaseFactor {
public:
    AwesomeoscFactor() : BaseFactor("AwesomeOsc") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for AwesomeOsc
        result_ = 0.0;
    }

private:
int window = 1;
};

REGISTER_FACTOR(AwesomeoscFactor);

#endif // AWESOMEOSC_H
