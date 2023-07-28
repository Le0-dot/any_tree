cmake_minimum_required(VERSION 3.27)

project(any_tree
	    VERSION 1.0.0
	    LANGUAGES CXX)

set(LIB any_tree)

add_library(${LIB} INTERFACE)

target_include_directories(${LIB} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_features(${LIB} INTERFACE cxx_std_20)
