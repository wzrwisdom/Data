file(GLOB TEST_SOURCES "${CMAKE_SOURCE_DIR}/tests/test_*.cpp")

#创建测试可执行文件
add_executable(FlowDataSystemTests test_main.cpp ${TEST_SOURCES})

# 链接动态库
target_link_libraries(FlowDataSystemTests PRIVATE FlowDataSystem)

# 添加头文件目录（测试也需包含include）
target_include_directories(FlowDataSystemTests PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
)


