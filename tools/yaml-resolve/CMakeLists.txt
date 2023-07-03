set(TARGET_NAME "yaml-resolve")

# Define the target executable
file(GLOB RESOLVER_SOURCES "src/*.cpp")
add_executable(${TARGET_NAME} ${RESOLVER_SOURCES})

target_include_directories(${TARGET_NAME} PUBLIC inc)

# Link against rapidyaml, fmt and Optimizer library
target_link_libraries(${TARGET_NAME} PUBLIC ryml::ryml fmt::fmt ${LIB_NAME})

# Include Lyra library
target_include_directories(${TARGET_NAME} PUBLIC ${PROJECT_DIR}/deps/Lyra/include)

# Add macro for Debug build
target_compile_definitions(${TARGET_NAME} PRIVATE "$<$<CONFIG:Debug>:YO_DEBUG>")
