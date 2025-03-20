cmake_minimum_required(VERSION 3.10)

project(DNA_Analysis VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(lab1
    src/lab1.cpp
    src/repeat.cpp
    src/stringProcess.cpp
)

target_include_directories(lab1 PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(lab1 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)

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

