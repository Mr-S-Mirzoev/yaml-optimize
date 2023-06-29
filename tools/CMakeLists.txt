cmake_minimum_required(VERSION 3.15)
project(yaml-tools)

set(CMAKE_CXX_STANDARD 20)

# Build Optimizer
add_subdirectory(yaml-optimize)

# Build Resolver
add_subdirectory(yaml-resolve)
