cmake_minimum_required(VERSION 3.15)
project(yaml-optimizer-tests)

set(CMAKE_CXX_STANDARD 20)

Include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.0.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

# Define the target for unit tests
add_executable(unittest
    src/full.unittest.cpp
    src/limit.unittest.cpp
)

# Link against Catch2
target_link_libraries(unittest PRIVATE Catch2::Catch2WithMain ${LIB_NAME})

# Set C++ standard to C++20
target_compile_features(unittest PRIVATE cxx_std_20)

# Set the include directories
target_include_directories(unittest PRIVATE ${PROJECT_DIR}/optimizer/inc)

# Add a test target for running the unit tests with ctest
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(unittest)
