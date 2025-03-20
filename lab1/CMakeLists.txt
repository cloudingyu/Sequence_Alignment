cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(DNA_Analysis VERSION 1.0)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Lab1 代码编译
add_executable(lab1
    src/lab1.cpp
    src/repeat.cpp
)

# 设置头文件路径
target_include_directories(lab1 PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# 设置输出文件路径
set_target_properties(lab1 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)

# 复制输入文件到输出目录
add_custom_command(TARGET lab1 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/sequence/reference.in
    ${CMAKE_CURRENT_SOURCE_DIR}/bin/reference.in
)

add_custom_command(TARGET lab1 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/sequence/query.in
    ${CMAKE_CURRENT_SOURCE_DIR}/bin/query.in
)

