#pragma once


#ifdef _WIN32
// MSVC gets confused when constexpr removes logic branches,
// and reports unreachable code: C4702

#define CONSTEXPR_SHIM_PUSH \
    __pragma(warning(push)) \
    __pragma(warning(disable:4702))

#define CONSTEXPR_SHIM_POP \
    __pragma(warning(pop))

#else

// clang and gcc are not confused
#define CONSTEXPR_SHIM_PUSH
#define CONSTEXPR_SHIM_POP

#endif


