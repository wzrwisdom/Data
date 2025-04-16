#ifndef WB1_TSRANK_H
#define WB1_TSRANK_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class Wb1TsrankFactor : public BaseFactor {
public:
    Wb1TsrankFactor() : BaseFactor("wb1_tsrank") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto wb1 = builder("wb1", window);
        if (wb1.size() < std::floor(window/3)) {
            result_ = std::nan("");
            return;
        }

        result_ = rank(wb1, window);
    }

private:
    int window = 1;
};

REGISTER_FACTOR(Wb1TsrankFactor);

#endif // WB1_TSRANK_H
