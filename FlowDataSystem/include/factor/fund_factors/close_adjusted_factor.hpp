#ifndef CLOSE_ADJUSTED_H
#define CLOSE_ADJUSTED_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class CloseAdjustedFactor : public BaseFactor {
public:
    CloseAdjustedFactor() : BaseFactor("close_adjusted") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto close = builder("close", window);
        if (close.size() < window) {
            result_ = std::nan("");
            return;
        }
        result_ = close.back() - mean(close, window) / stdev(close, window);
    }

private:
    int window = 1;
};

REGISTER_FACTOR(CloseAdjustedFactor);

#endif // CLOSE_ADJUSTED_H
