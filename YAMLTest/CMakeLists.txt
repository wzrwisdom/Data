cmake_minimum_required(VERSION 3.10)
project(MyYamlProject)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)

# 设置 yaml-cpp 路径（你自己替换成你本地路径）
set(YAML_CPP_DIR "D:/Documents/code/CPP_LIBRARY/yaml-cpp")
set(YAML_CPP_INCLUDE_DIR "${YAML_CPP_DIR}/include")
set(YAML_CPP_LIBRARY "${YAML_CPP_DIR}/build/libyaml-cpp.dll")
# set(YAML_CPP_LIBRARY "${YAML_CPP_DIR}/bin/yaml-cpp.dll")

# 添加头文件路径
include_directories(${YAML_CPP_INCLUDE_DIR})

# 添加可执行文件
add_executable(MyYamlProject main.cpp)
# target_compile_definitions(MyYamlProject PRIVATE YAML_CPP_STATIC_DEFINE)

# 链接 yaml-cpp 库
target_link_libraries(MyYamlProject ${YAML_CPP_LIBRARY})
