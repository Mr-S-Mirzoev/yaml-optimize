set(TARGET_NAME "yaml-resolve")

# Define the target executable
add_executable(${TARGET_NAME} src/main.cpp)

target_include_directories(${TARGET_NAME} PUBLIC ${PROJECT_DIR}/src/inc)

# Link against rapidyaml library
target_link_libraries(${TARGET_NAME} PUBLIC ryml::ryml)
target_include_directories(${TARGET_NAME} PRIVATE ${PROJECT_DIR}/deps/clara)

target_link_libraries(${TARGET_NAME} PUBLIC fmt::fmt)

# Add macro for Debug build
target_compile_definitions(${TARGET_NAME} PRIVATE "$<$<CONFIG:Debug>:YO_DEBUG>")
