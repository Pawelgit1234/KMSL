cmake_minimum_required(VERSION 3.14)
project(KMSL VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/**/*.cpp"
    "src/*.hpp"
    "src/**/*.hpp"
)

add_executable(KMSL ${SOURCES})

target_include_directories(KMSL PRIVATE ${PROJECT_SOURCE_DIR}/src)
