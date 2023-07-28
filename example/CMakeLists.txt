cmake_minimum_required(VERSION 3.27)

project(demo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(demo demo.cpp)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR}/any_tree)

target_link_libraries(demo PUBLIC any_tree)
