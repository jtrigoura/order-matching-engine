cmake_minimum_required(VERSION 3.16)
project(order_matching_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(order_matching_engine
    src/main.cpp
    src/order_book.cpp
)

target_include_directories(order_matching_engine
    PRIVATE include
)
