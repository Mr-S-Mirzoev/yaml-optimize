cmake_minimum_required(VERSION 3.15)
project(yaml-optimize)

set(CMAKE_CXX_STANDARD 20)

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
target_compile_definitions(${TARGET_NAME} PRIVATE "$<$<CONFIG:Debug>:YO_DEBUG>")

# Add testing utils
add_subdirectory(test)
