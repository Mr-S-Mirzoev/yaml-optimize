cmake_minimum_required(VERSION 3.15)
project(yaml-utils)

set(CMAKE_CXX_STANDARD 20)

set(PROJECT_DIR ${PROJECT_SOURCE_DIR})

# Add the fmt library for fast string formatting
add_subdirectory(deps/fmt)

# Add the RapidYaml library for fast YAML processing
add_subdirectory(deps/rapidyaml)

set(LIB_NAME yaml-optimizer-lib)

# Add optimization library as a dependency
add_subdirectory(src)

if (YO_BUILD_TESTS)
    add_subdirectory(test)
endif(YO_BUILD_TESTS)

if (YO_BUILD_TOOLS)
    # Add tools (e.g. YAML resolver)
    add_subdirectory(tools)
endif(YO_BUILD_TOOLS)
