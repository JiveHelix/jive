#pragma once



#ifdef __clang__
#define CLANG_COMPILER
#elif defined(__GNUC__)
#define GCC_COMPILER
#elif defined(_MSC_VER)
#define MSC_COMPILER
#endif
