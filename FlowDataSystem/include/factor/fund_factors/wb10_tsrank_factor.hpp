#ifndef WB10_TSRANK_H
#define WB10_TSRANK_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class Wb10TsrankFactor : public BaseFactor {
public:
    Wb10TsrankFactor() : BaseFactor("wb10_tsrank") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto wb10 = builder("wb10", window);
        if (wb10.size() < std::floor(window/3)) {
            result_ = std::nan("");
            return;
        }

        result_ = ts_rank(wb10, window);
    }

private:
    int window = 1;
};

REGISTER_FACTOR(Wb10TsrankFactor);

#endif // WB10_TSRANK_H
