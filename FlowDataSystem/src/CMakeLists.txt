file(GLOB_RECURSE SOURCES
    "core/*.cpp"
    "feature/*.cpp"
    "factor/*.cpp"
    "factor/"

)

# 收集所有头文件
file(GLOB_RECURSE HEADERS
    "${CMAKE_SOURCE_DIR}/include/*.hpp"
)

# 创建库目标
add_library(FlowDataSystem STATIC ${SOURCES} ${HEADERS})
# add_library(FlowDataSystem SHARED ${SOURCES} ${HEADERS})

# # 设置目标属性
# target_include_directories(FlowDataSystem 
#     PUBLIC 
#         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
#         $<INSTALL_INTERFACE:include>
#     PRIVATE
#         ${CMAKE_CURRENT_SOURCE_DIR}
# )
target_include_directories(FlowDataSystem PRIVATE ${CMAKE_SOURCE_DIR}/include)  # 添加头文件目录
target_include_directories(FlowDataSystem PRIVATE ${TALIB_INCLUDE_DIR})  # 添加TA-Lib头文件目录
target_include_directories(FlowDataSystem PRIVATE ${YAML_CPP_INCLUDE_DIR})  # 添加yaml-cpp头文件目录

# 链接依赖库
target_link_libraries(FlowDataSystem
    PUBLIC
        ${YAML_CPP_LIBRARIES}  # 链接yaml-cpp库
        ${TALIB_LIBRARIES}
)

# 安装规则
install(TARGETS FlowDataSystem
    EXPORT FlowDataSystemTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY ../include/ DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)

install(EXPORT FlowDataSystemTargets
    FILE FlowDataSystemTargets.cmake
    NAMESPACE FlowDataSystem::
    DESTINATION lib/cmake/FlowDataSystem
)

# include_directories(${CMAKE_SOURCE_DIR}/include)

# file(GLOB_RECURSE CORE_SRC_LIST "core/*.cpp") 
# file(GLOB_RECURSE FEATURE_SRC_LIST "feature/*.cpp") 
# file(GLOB_RECURSE FACTOR_SRC_LIST "factor/*.cpp") 


# # 添加源文件路径
# add_library(CoreLib
#     ${CORE_SRC_LIST}
# )
# add_library(FeatLib
#     ${FEATURE_SRC_LIST}
# )
# add_library(FacLib
#     ${FACTOR_SRC_LIST}
# )
# target_link_libraries(FacLib PRIVATE yaml-cpp)  # 链接库

# add_library(FlowDataSystem SHARED ${CORE_SRC_FILE} ${FEATURE_SRC_LIST} ${FACTOR_SRC_LIST})  # 创建动态库
# target_include_directories(FlowDataSystem PUBLIC ${CMAKE_SOURCE_DIR}/include)  # 添加头文件目录
# target_include_directories(FlowDataSystem PRIVATE ${TALIB_INCLUDE_DIR})  # 添加TA-Lib头文件目录
# target_link_libraries(FlowDataSystem 
#     PRIVATE yaml-cpp  # 链接yaml-cpp库
#     PRIVATE ${TALIB_LIBRARIES} # 链接TA-Lib库
# )  