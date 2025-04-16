#ifndef FACTOR_FACTORY_H
#define FACTOR_FACTORY_H

#include "factor/base.hpp"
#include <string>
#include <unordered_map>
#include <functional>
#include <memory>

class FactorFactory {
public:
    static FactorFactory& instance();

    void registerFactor(const std::string& name, std::function<std::shared_ptr<BaseFactor>()> creator);

    std::shared_ptr<BaseFactor> create(const std::string& name);

private:
    // 构造函数，默认初始化
    FactorFactory() = default;

    std::unordered_map<std::string, std::function<std::shared_ptr<BaseFactor>()>> registry;
};

#define REGISTER_FACTOR(classname) \
    namespace { \
        struct classname##Registrator { \
            classname##Registrator() { \
                FactorFactory::instance().registerFactor(#classname, []() { \
                    return std::make_shared<classname>(); \
                }); \
            } \
        }; \
        static classname##Registrator global_##classname##Registrator; \
    }

#endif // FACTOR_FACTORY_H
