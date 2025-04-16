#ifndef LI_FUTURE_H
#define LI_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class LiFutureFactor : public BaseFactor {
public:
    LiFutureFactor() : BaseFactor("LI_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto last = builder("last", window);
        if (last.size() < std::floor(window/3)) {
            result_ = std::nan("");
            return;
        }
        result_ = stdev(last, window) / mean(last, window);
    }

private:
    int window = 1;
};

REGISTER_FACTOR(LiFutureFactor);

#endif // LI_FUTURE_H
