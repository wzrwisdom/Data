cmake_minimum_required(VERSION 3.10)
project(FlowDataSystem
    VERSION 1.0
    DESCRIPTION "A quantitative finance system with multi-factors"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)

# 设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 禁用 #pragma message 相关的警告
add_definitions(-DYAML_CPP_STATIC_DEFINE)
# 查找依赖包

set(YAML_CPP_DIR "D:/Documents/code/CPP_LIBRARY/yaml-cpp")
# find_package(yaml-cpp REQUIRED PATHS "${YAML_CPP_DIR}/share")
set(YAML_CPP_INCLUDE_DIR "${YAML_CPP_DIR}/include")
set(YAML_CPP_LIBRARIES "${YAML_CPP_DIR}/build/libyaml-cpp.dll")

# Specify the directory containing TA_LIB configuration files
set(TALIB_DIR "D:/Program Files/vcpkg/packages/talib_x64-windows")
set(TALIB_INCLUDE_DIR "${TALIB_DIR}/include")
set(TALIB_LIBRARIES     "${TALIB_DIR}/lib/ta_libc.lib")
# find_path(TALIB_INCLUDE_DIR ta_libc.h PATHS ${TALIB_DIR}/include)

# set(TALIB_LIBRARIES 
#     ${TALIB_DIR}/lib/ta_abstract.lib
#     ${TALIB_DIR}/lib/ta_func.lib
#     ${TALIB_DIR}/lib/ta_common.lib    
# )

# 添加 Pybind11
set(PYBIND11_DIR "C:/Users/12552/Documents/CPP_LIBRARY/pybind11")
set(PYBIND11_INCLUDE_DIR "${PYBIND11_DIR}/include")
add_subdirectory("pybind11")
pybind11_add_module(flow_data_system src/bindings.cpp)

# message(STATUS "pybind_INCLUDE_DIRS: ${PYBIND11_INCLUDE_DIR}")
if(NOT YAML_CPP_INCLUDE_DIR OR NOT YAML_CPP_LIBRARIES)
    message(STATUS "yaml-cpp_INCLUDE_DIRS: ${YAML_CPP_INCLUDE_DIR} ${YAML_CPP_LIBRARIES}")
endif()
if(NOT TALIB_INCLUDE_DIR OR NOT TALIB_LIBRARIES)
    # message(FATAL_ERROR "Could not find TA-Lib")
    message(STATUS "Not found ${TALIB_INCLUDE_DIR} ${TALIB_LIBRARIES}")
endif()


# 添加源文件
add_subdirectory(src)


# 项目配置选项
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()    
    add_subdirectory(tests)
endif()
