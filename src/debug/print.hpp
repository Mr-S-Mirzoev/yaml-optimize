#pragma once

#ifndef YO_DEBUG

#define DEBUG_PRINT(...)

#else

#include <fmt/format.h>

#define DEBUG_PRINT(message, ...)                                              \
    fmt::print("[DEBUG] " message "\n", ##__VA_ARGS__)

#endif // YO_DEBUG
