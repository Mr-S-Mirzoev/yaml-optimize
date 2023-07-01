add_library(${LIB_NAME} STATIC optimizer.cpp)

target_include_directories(${LIB_NAME} PUBLIC inc)

# Link against rapidyaml library
target_link_libraries(${LIB_NAME} PUBLIC ryml::ryml)
target_include_directories(${LIB_NAME} PUBLIC ${PROJECT_DIR}/deps/Lyra/include)

target_link_libraries(${LIB_NAME} PUBLIC fmt::fmt)

# Add macro for Debug build
target_compile_definitions(${LIB_NAME} PRIVATE "$<$<CONFIG:Debug>:YO_DEBUG>")
