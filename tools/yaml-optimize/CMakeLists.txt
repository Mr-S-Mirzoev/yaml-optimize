set(TARGET_NAME "yaml-optimize")

# Define the target executable
file(GLOB OPTIMIZER_SOURCES "src/*.cpp")
add_executable(${TARGET_NAME} ${OPTIMIZER_SOURCES})

target_include_directories(${TARGET_NAME} PUBLIC inc)

# Link against Optimizer library
target_link_libraries(${TARGET_NAME} ${LIB_NAME})

# Add macro for Debug build
target_compile_definitions(${TARGET_NAME} PRIVATE "$<$<CONFIG:Debug>:YO_DEBUG>")
