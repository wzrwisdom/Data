#include <iostream>
#include <yaml-cpp/yaml.h>

int main() {
    // 加载YAML配置文件
    YAML::Node config = YAML::Load("{name: '王子睿', age: 28}");
    std::cout << "Name: " << config["name"].as<std::string>() << std::endl;
    std::cout << "Age: " << config["age"].as<int>() << std::endl;
    return 0;
}