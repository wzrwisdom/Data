#ifndef BS_PV1_TSRANK_FUTURE_H
#define BS_PV1_TSRANK_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class BsPv1TsrankFutureFactor : public BaseFactor {
public:
    BsPv1TsrankFutureFactor() : BaseFactor("bs_pv1_tsrank_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto b1 = builder("b1", window);
        auto s1 = builder("s1", window);
        auto bv1 = builder("bv1", window);
        auto sv1 = builder("sv1", window);

        if (b1.size() < std::floor(window/3)) {
            result_ = std::nan("");
            return;
        }
        result_ = rank(b1*bv1 - s1*sv1, window);
    }

private:
int window = 1;
};

REGISTER_FACTOR(BsPv1TsrankFutureFactor);

#endif // BS_PV1_TSRANK_FUTURE_H
