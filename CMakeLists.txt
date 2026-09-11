cmake_minimum_required(VERSION 3.10)
project(cs2310-group LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cs2310-group
    src/main.cpp
    src/GCD.cpp
)