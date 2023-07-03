#pragma once

/* clang-format off */
#if defined(__APPLE__) && defined(__MACH__)
    // Apple platforms
    #define YO_APPLE

    #include <TargetConditionals.h>
    #if TARGET_OS_MAC
        // macOS
        #define YO_MACOS
    #endif

    #if TARGET_OS_IOS
        // iOS
        #define YO_IOS
    #endif

    #if TARGET_OS_TV
        // tvOS
        #define YO_TVOS
    #endif

    #if TARGET_OS_WATCH
        // watchOS
        #define YO_WATCHOS
    #endif
#elif defined(_WIN32) || defined(_WIN64)
    // Windows platforms
    #define YO_WINDOWS

    #ifdef _WIN32
        #define YO_WINDOWS32
    #endif

    #ifdef _WIN64
        #define YO_WINDOWS64
    #endif
#elif defined(__linux__)
    // Linux platform
    #define YO_LINUX
#elif defined(__ANDROID__)
    // Android platform
    #define YO_ANDROID
#elif defined(__FreeBSD__)
    // FreeBSD platform
    #define YO_FREEBSD
#elif defined(__unix__) || defined(__unix)
    // Generic Unix platform
    #define YO_UNIX
#else
    // Unsupported platform
    #error Unsupported platform
#endif
/* clang-format off */
