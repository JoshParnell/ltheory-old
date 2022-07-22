#ifndef Common_h__
#define Common_h__

#include <cfloat>
#include <climits>
#include <cstddef>
#include <stdint.h>
#include <stdlib.h>

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

typedef int32_t  int32;
typedef uint32_t uint32;
typedef int64_t  int64;
typedef uint64_t uint64;

#define LIBLT
#define LIBLT_VERSION_MAJOR 0
#define LIBLT_VERSION_MINOR 1

/* Detect OS. */
#if defined(_WIN32) || defined(__WIN32__)

  #define LIBLT_WINDOWS

#elif defined(linux) || defined(__linux)

  #define LIBLT_LINUX

#elif defined(__APPLE__) || defined(MACOSX) || defined(macintosh) || defined(Macintosh)

  #define LIBLT_MACOS

#else

  #error Unsupported operating system

#endif

#ifdef LIBLT_WINDOWS

  #define LT_API __declspec(dllexport)

  /* Macro redefinition warning. */
  #pragma warning(disable : 4005)

  /* Warning about not having debug information for certain libs. */
  #pragma warning(disable : 4099)

  /* 'this' used in base member initializer list */
  #pragma warning(disable : 4355)

  /* Warning about not enabling exceptions. */
  #pragma warning(disable : 4530)

  /* Multiple copy constructor warning. */
  #pragma warning(disable : 4521)

  /* CRT depracation warnings. */
  #pragma warning(disable : 4996)

  /* Forcing value to bool performance wardning. */
  #pragma warning(disable : 4800)

  /* Treat implict type conversions as an error. */
  // #pragma warning(error: 4244)

  /* Deleting an incomplete type is an error. */
  #pragma warning(error: 4150)

  /* Not all control paths return a value is an error. */
  #pragma warning(error: 4715)

  /* Incorrect number of macro parameters is an error. */
  #pragma warning(error: 4002)

#else

  #define LT_API

#endif

/* Detect architecture. */
#if _WIN32 || _WIN64

  #if _WIN32

    #define ARCH_32

  #else

    #define ARCH_64

  #endif

#elif  __GNUC__

  #if __x86_64__

    #define ARCH_64

  #else

    #define ARCH_32

  #endif

#else

  #error Unsupported architecture

#endif

LT_API void LTE_ASSERT_FAILURE(
  char const* file,
  int line,
  char const* statement);

LT_API void LTE_DEBUG_BREAK(char const* file, int line);

LT_API void LTE_PRINT_FILE(char const* file, int line);

#define LTE_ASSERT(x) if (!(x)) { LTE_ASSERT_FAILURE(__FILE__, __LINE__, #x); }

#define error(x) { LTE_ASSERT_FAILURE(__FILE__, __LINE__, #x); }

#define debugbreak LTE_DEBUG_BREAK(__FILE__, __LINE__);
#define debugprint LTE_PRINT_FILE(__FILE__, __LINE__);

#define NOT_IMPLEMENTED                                                        \
  LTE_ASSERT_FAILURE(__FILE__, __LINE__, "Invoking Unimplemented Function");

#endif
