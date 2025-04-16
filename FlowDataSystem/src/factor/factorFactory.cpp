#include "factor/factorFactory.hpp"
#include <stdexcept>

FactorFactory& FactorFactory::instance() {
    static FactorFactory factory;
    return factory;
}

void FactorFactory::registerFactor(const std::string& name, std::function<std::shared_ptr<BaseFactor>()> creator) {
    registry[name] = creator;
}

std::shared_ptr<BaseFactor> FactorFactory::create(const std::string& name) {
    auto it = registry.find(name);
    if (it != registry.end()) {
        return it->second();
    }
    throw std::runtime_error("Factor not found in factory: " + name);
}
