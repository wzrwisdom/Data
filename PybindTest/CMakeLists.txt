cmake_minimum_required(VERSION 3.10)
project(my_pybind_project)

set(CMAKE_CXX_STANDARD 17)



# 设置 Python 环境（自动获取 Python 安装信息）
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

# 包含 pybind11（如果是源码下载）
set(PYBIND11_CPP_STANDARD -std=c++17)
set(PYBIND11_INSTALL OFF)
add_subdirectory(pybind11)
set_target_properties(pybind11::module PROPERTIES IMPORTED_IMPLIB "")

# 创建 Python 模块（共享库）
pybind11_add_module(mymodule main.cpp)

# 链接 Python3
target_link_libraries(mymodule PRIVATE Python3::Python)

