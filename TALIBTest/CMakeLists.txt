cmake_minimum_required(VERSION 3.10)
project(MyTaLibProject)

set(CMAKE_CXX_STANDARD 17)

# 设置 TA-Lib 的路径（替换成你本地路径）
set(TALIB_DIR "D:/Program Files/vcpkg/packages/talib_x64-windows")
set(TALIB_INCLUDE_DIR "${TALIB_DIR}/include")
set(TALIB_LIBRARIES     "${TALIB_DIR}/lib/ta_libc.lib")
# # 设置 TA-Lib 的路径（替换成你本地路径）
# set(TALIB_DIR "C:/Users/12552/Documents/CPP_LIBRARY/ta-lib")
# set(TALIB_INCLUDE_DIR "${TALIB_DIR}/include")
# set(TALIB_LIBRARIES     "${TALIB_DIR}/build/ta-lib.lib")
# message(STATUS ${TALIB_LIBRARIES} ${TALIB_INCLUDE_DIR})

# 添加头文件路径
include_directories(${TALIB_INCLUDE_DIR})

# 可执行程序
add_executable(MyTaLibProject main.cpp)

# 链接 TA-Lib
target_link_libraries(MyTaLibProject ${TALIB_LIBRARIES})
