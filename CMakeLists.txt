cmake_minimum_required(VERSION 3.15)
project(yaml-optimize)

set(CMAKE_CXX_STANDARD 20)

# Set the default build type to Release if not specified
set(YO_DEBUG $<$<CONFIG:Debug>:true>)
if(NOT YO_DEBUG)
    set(YO_DEBUG false)
    set(YO_BUILD_TYPE release)
else()
    set(YO_BUILD_TYPE debug)
endif()

# Set the output directories based on the build type
set(BUILD_DIR "${CMAKE_BINARY_DIR}/build_${YO_BUILD_TYPE}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin_${YO_BUILD_TYPE}")

# Add the rapidyaml library
add_subdirectory(deps/rapidyaml)

# Define the target executable
add_executable(yaml-optimize src/main.cpp)

# Link against rapidyaml library
target_link_libraries(yaml-optimize PRIVATE ryml::ryml)
target_include_directories(yaml-optimize PRIVATE deps/clara)

# Add fmt library as a dependency for Debug build
if(YO_DEBUG)
    add_subdirectory(deps/fmt)
    target_compile_definitions(yaml-optimize PRIVATE "YO_DEBUG")
    target_link_libraries(yaml-optimize PRIVATE fmt::fmt)
endif()
