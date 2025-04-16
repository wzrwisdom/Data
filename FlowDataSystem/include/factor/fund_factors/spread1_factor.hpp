#ifndef SPREAD1_H
#define SPREAD1_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class Spread1Factor : public BaseFactor {
public:
    Spread1Factor() : BaseFactor("spread1") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        // No parameters
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto s1 = builder("s1", 1);
        auto b1 = builder("b1", 1);
        if (s1.size() < 1) {
            result_ = std::nan("");
            return;
        }
        result_ = s1[0] - b1[0];
    }

private:

};

REGISTER_FACTOR(Spread1Factor);

#endif // SPREAD1_H
