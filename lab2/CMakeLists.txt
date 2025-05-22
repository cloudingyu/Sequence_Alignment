cmake_minimum_required(VERSION 3.10)

project(Lab2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(lab2
    src/lab2main.cpp
    src/fileProcess.cpp
)

target_include_directories(lab2 PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(lab2 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_custom_target(copy_files ALL
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/sequence/reference1.in ${CMAKE_CURRENT_SOURCE_DIR}/bin/reference1.in
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/sequence/query1.in ${CMAKE_CURRENT_SOURCE_DIR}/bin/query1.in
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/sequence/reference2.in ${CMAKE_CURRENT_SOURCE_DIR}/bin/reference2.in
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/sequence/query2.in ${CMAKE_CURRENT_SOURCE_DIR}/bin/query2.in
    DEPENDS lab2
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/result)
