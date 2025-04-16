#ifndef VLI_FUTURE_H
#define VLI_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class VliFutureFactor : public BaseFactor {
public:
    VliFutureFactor() : BaseFactor("VLI_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto vol = builder("vol", window);
        if (vol.size() < std::floor(window/3)) {
            result_ = 0;
            return;
        }
        result_ = stdev(vol, window) / mean(vol, window);
        result_ = std::isnan(result_) ? 0 : result_;
    }

private:
    int window = 1;
};

REGISTER_FACTOR(VliFutureFactor);

#endif // VLI_FUTURE_H
