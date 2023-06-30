#pragma once

#include "fmt/format.h"

#define YO_THROW(exception_type, message, ...)                                 \
    throw exception_type(fmt::format(message, ##__VA_ARGS__));
