#ifndef CLH_FUTURE_H
#define CLH_FUTURE_H

#include "factor/base.hpp"
#include "factor/factorFactory.hpp"

class ClhFutureFactor : public BaseFactor {
public:
    ClhFutureFactor() : BaseFactor("clh_future") {}

    void setParams(const std::unordered_map<std::string, double>& params) override {
        if (params.count("window")) window = static_cast<int>(params.at("window"));
    }

    void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) override {
        auto last = builder("last", window);
        if (last.size() < std::min(static_cast<int>(std::floor(window / 2)), 100)) {
            result_ = std::nan("");
            return;
        }
        double c = last.back();
        double h = *std::max_element(last.begin(), last.end());
        double l = *std::min_element(last.begin(), last.end());
        result_ = (h - l) != 0 ?  (h+l-2*c)/ (h - l) : 0;
    }

private:
    int window = 1;
};

REGISTER_FACTOR(ClhFutureFactor);

#endif // CLH_FUTURE_H
