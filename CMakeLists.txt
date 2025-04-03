cmake_minimum_required(VERSION 3.20)
project(AIZO_STylinski)

set(CMAKE_CXX_STANDARD 20)

add_executable(AIZO_STylinski
        main.cpp
        cmake-build-debug/src/BatchTests.cpp
        cmake-build-debug/include/BatchTests.hpp
        src/BatchTests.cpp
)

target_include_directories(AIZO_STylinski PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
