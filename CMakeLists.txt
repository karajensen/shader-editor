cmake_minimum_required(VERSION 3.17)
project(ShaderEditor)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

add_subdirectory(src)
