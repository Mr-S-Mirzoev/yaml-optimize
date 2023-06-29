cmake_minimum_required(VERSION 3.15)
project(yaml-utils)

set(CMAKE_CXX_STANDARD 20)

set(PROJECT_DIR ${PROJECT_SOURCE_DIR})

# Add the rapidyaml library
add_subdirectory(deps/rapidyaml)

# Add fmt library as a dependency
add_subdirectory(deps/fmt)

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
