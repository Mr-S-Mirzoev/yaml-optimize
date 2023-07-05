add_library(${LIB_NAME} INTERFACE)

target_include_directories(${LIB_NAME} INTERFACE inc)
target_include_directories(${LIB_NAME} INTERFACE src)

# Link against rapidyaml library
target_link_libraries(${LIB_NAME} INTERFACE ryml::ryml)
target_include_directories(${LIB_NAME} INTERFACE ${PROJECT_DIR}/deps/Lyra/include)

target_link_libraries(${LIB_NAME} INTERFACE fmt::fmt)

# Add macro for Debug build
target_compile_definitions(${LIB_NAME} INTERFACE "$<$<CONFIG:Debug>:YO_DEBUG>")
