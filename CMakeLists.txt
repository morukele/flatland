cmake_minimum_required(VERSION 4.1)
project(flatland)

set(CMAKE_CXX_STANDARD 23)

add_executable(flatland main.cpp src/flatlandien.cpp)

target_include_directories(flatland PRIVATE include)