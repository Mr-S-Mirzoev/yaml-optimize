set(TARGET_NAME "yaml-optimize")

# Define the target executable
add_executable(${TARGET_NAME} main.cpp)

target_link_libraries(${TARGET_NAME} ${LIB_NAME})

target_include_directories(${TARGET_NAME} PUBLIC ${PROJECT_DIR}/src/inc)

# Add macro for Debug build
target_compile_definitions(${TARGET_NAME} PRIVATE "$<$<CONFIG:Debug>:YO_DEBUG>")
