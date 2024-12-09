project(aoc_2024 LANGUAGES CXX)

cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(DIRECTORIES 
    "01"
    "02"
    "03"
    "04"
    "05"
    "06"
    "07"
    "08"
    "09"
    "10"
    "11"
    "12"
    "13"
    "14"
    "15"
    "16"
    "17"
    "19"
    "20"
    "21"
    "22"
    "23"
    "24"
    "25"
)

foreach(dir ${DIRECTORIES})
    add_subdirectory(${dir})
endforeach()