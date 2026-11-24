cmake_minimum_required(VERSION 3.10)
project(cs2310-group LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(grade
    src/main.cpp
    src/store.cpp
    src/calc.cpp
    src/report.cpp
)

target_include_directories(grade PRIVATE include libs)

include(CTest)
if(BUILD_TESTING)
    add_executable(report_tests src/report_tests.cpp src/calc.cpp src/report.cpp)
    target_include_directories(report_tests PRIVATE include libs)
    add_test(NAME report_tests COMMAND report_tests)
endif()
