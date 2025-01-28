cmake_minimum_required(VERSION 3.14)
project(KMSL VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Boost REQUIRED COMPONENTS program_options)

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Boost not found")
endif()

file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/**/*.cpp"
    "src/*.hpp"
    "src/**/*.hpp"
)

add_executable(KMSL ${SOURCES})

target_include_directories(KMSL PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(KMSL PRIVATE Boost::program_options)
