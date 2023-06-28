cmake_minimum_required(VERSION 3.15)
project(yaml-optimize)

set(CMAKE_CXX_STANDARD 20)

# Set the default build type to Release if not specified
set(YO_DEBUG $<$<CONFIG:Debug>:true>)
if(NOT YO_DEBUG OR NOT ${YO_DEBUG})
    set(YO_DEBUG false)
    set(YO_BUILD_TYPE release)
else()
    set(YO_DEBUG true)
    set(YO_BUILD_TYPE debug)
endif()

# Set the output directories based on the build type
set(BUILD_DIR "${CMAKE_BINARY_DIR}/build_${YO_BUILD_TYPE}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin_${YO_BUILD_TYPE}")

set(TARGET_NAME "${PROJECT_NAME}")

# Add the rapidyaml library
add_subdirectory(deps/rapidyaml)

# Add fmt library as a dependency
add_subdirectory(deps/fmt)

# Define the target executable
add_executable(${TARGET_NAME} src/main.cpp)

# Link against rapidyaml library
target_link_libraries(${TARGET_NAME} PRIVATE ryml::ryml)
target_include_directories(${TARGET_NAME} PRIVATE deps/clara)

target_link_libraries(${TARGET_NAME} PRIVATE fmt::fmt)

# Add macro for Debug build
if(YO_DEBUG)
    target_compile_definitions(${TARGET_NAME} PRIVATE "YO_DEBUG")
endif()

# Add testing utils
add_subdirectory(test)
