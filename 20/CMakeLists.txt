project(aoc_20 LANGUAGES CXX)

cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(aoc_20_1 ${CMAKE_CURRENT_LIST_DIR}/part_1/main.cpp)
add_executable(aoc_20_2 ${CMAKE_CURRENT_LIST_DIR}/part_2/main.cpp)
