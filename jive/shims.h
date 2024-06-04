#pragma once


#if defined _WIN32 && !(defined __MINGW32__ || __MINGW64__)

#define NLOHMANN_PUSH_IGNORES \
    __pragma(warning(push)) \
    __pragma(warning(disable:26819))

#define NLOHMANN_POP_IGNORES \
    __pragma(warning(pop))


#define FMT_PUSH_IGNORES \
    __pragma(warning(push)) \
    __pragma(warning(disable:6285))

#define FMT_POP_IGNORES \
    __pragma(warning(pop))

#else

#define NLOHMANN_PUSH_IGNORES
#define NLOHMANN_POP_IGNORES

#define FMT_PUSH_IGNORES
#define FMT_POP_IGNORES

#endif
