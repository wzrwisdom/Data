#ifndef SPREAD1_H
#define SPREAD1_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"
#include "factor/ts_utils.hpp"

class Spread1Factor : public BaseFactor {
public:
    Spread1Factor() : BaseFactor("spread1") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
// No parameters
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        // TODO: implement logic for spread1
        result_ = 0.0;
    }

private:

};

REGISTER_FACTOR(Spread1Factor);

#endif // SPREAD1_H
