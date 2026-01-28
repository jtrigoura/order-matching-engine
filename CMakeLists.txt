cmake_minimum_required(VERSION 3.16)
project(order_matching_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Core library
add_library(ome
    src/order_book.cpp
    src/engine.cpp
)

target_include_directories(ome
    PUBLIC include
)

# Main executable
add_executable(order_matching_engine
    src/main.cpp
)

target_link_libraries(order_matching_engine
    PRIVATE ome
)

# ----------------------------
# Tests
# ----------------------------
include(CTest)
enable_testing()

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

# For Windows; harmless elsewhere
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(ome_tests
    tests/order_book_tests.cpp
    tests/engine_tests.cpp
)

target_link_libraries(ome_tests
    PRIVATE ome GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(ome_tests)

