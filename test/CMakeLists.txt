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
file(GLOB UNIT_TEST_SOURCES "${PROJECT_DIR}/test/**/*.unittest.cpp")
add_executable(unittest ${UNIT_TEST_SOURCES})

# Link against Catch2
target_link_libraries(unittest PRIVATE Catch2::Catch2WithMain ${LIB_NAME})

# Set C++ standard to C++20
target_compile_features(unittest PRIVATE cxx_std_20)

# Set the include directories
target_include_directories(unittest PRIVATE ${PROJECT_DIR}/src/inc)

# Define the target for perf tests
file(GLOB PERF_TEST_SOURCES "${PROJECT_DIR}/test/**/*.perftest.cpp")
add_executable(perftest perftest.cpp ${PERF_TEST_SOURCES})

# Link against Celero
target_link_libraries(perftest PRIVATE celero ${LIB_NAME})

# Set C++ standard to C++20
target_compile_features(perftest PRIVATE cxx_std_20)

# Include Celero directories
target_include_directories(perftest PRIVATE ${PROJECT_DIR}/deps/Celero/include)

# Include Project directories
target_include_directories(perftest PRIVATE ${PROJECT_DIR}/src/inc)

# Add a test target for running the unit tests with ctest
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(unittest)

# Add a custom target for running the performance tests with ctest
add_test(
    NAME perftest
    COMMAND perftest
)
