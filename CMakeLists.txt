cmake_minimum_required(VERSION 3.27)
project(BucketStorage)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(BucketStorage
        bucket_storage.hpp
        tests.cpp)

add_executable(example tests.cpp)
target_link_libraries(example gtest_main)
add_test(NAME example_test COMMAND example)
