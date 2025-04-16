// base_factor.h
#ifndef BASE_FACTOR_HPP
#define BASE_FACTOR_HPP


#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

#include "core/talibfunction.hpp"
#include "core/function.hpp"

class BaseFactor {
public:
    BaseFactor(const std::string& name)
        : name_(name) {}

    virtual ~BaseFactor() = default;

    // Pure virtual method to be implemented by subclasses
    virtual void compute(const std::function<std::vector<double>(const std::string&, int)>& builder) = 0;

    const std::string& getName() const { return name_; }
    // const std::unordered_map<std::string, int>& getParams() const { return params_; }
    virtual void setParams(const std::unordered_map<std::string, double>& params) = 0;

    double getResult() const {
        return result_;
    }

protected:
    std::string name_;
    double result_ = std::nan("");
};

#endif // BASE_FACTOR_HPP
