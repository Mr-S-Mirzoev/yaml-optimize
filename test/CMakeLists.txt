set(TARGET_NAME "yaml-resolve")

# Define the target executable
add_executable(${TARGET_NAME} src/main.cpp)

# Link against rapidyaml library
target_link_libraries(${TARGET_NAME} PRIVATE ryml::ryml)
target_include_directories(${TARGET_NAME} PRIVATE ../deps/clara)

target_link_libraries(${TARGET_NAME} PRIVATE fmt::fmt)

# Add macro for Debug build
target_compile_definitions(${TARGET_NAME} PRIVATE "$<$<CONFIG:Debug>:YO_DEBUG>")
