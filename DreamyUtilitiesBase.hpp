//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_BASE_H
#define _DREAMYUTILITIES_INCL_BASE_H

// Current version of Dreamy Utilities
#define _DREAMY_UTIL_VER 0L

// Platform switch
#if defined(_WIN32) || defined(_MSC_VER)
  #define _DREAMY_UNIX 0 // Building for Windows

  // Check for modern C++
  #if !defined(_DREAMY_CPP11)
    #define _DREAMY_CPP11 (_MSC_VER >= 1800)
  #endif

#elif defined(__GNUC__) || defined(__unix__) || defined(__unix)
  #define _DREAMY_UNIX 1 // Building for Unix

  // Check for modern C++
  #if !defined(_DREAMY_CPP11)
    #if !defined(__cplusplus)
      #error Unknown C++ version ('__cplusplus' is undefined)
    #else
      #define _DREAMY_CPP11 (__cplusplus > 199711L)
    #endif
  #endif

#else
  #error Unsupported compiler.
#endif

#if !_DREAMY_CPP11 // Old C++

  // Don't notify about truncation of identifiers and decorated names
  #pragma warning(disable: 4786 4503)

  // Define null pointer
  #define nullptr 0

  #if !defined(final)
    // Non-overridable function/non-derivable class specifier
    #define final
  #endif

#endif

// Endianness
#define _DREAMY_LITTLE_ENDIAN 0
#define _DREAMY_BIG_ENDIAN    1

#if !defined(_DREAMY_BYTE_ORDER)
  // Assume little endian by default (x86 architecture)
  #define _DREAMY_BYTE_ORDER _DREAMY_LITTLE_ENDIAN
#endif

#if !_DREAMY_UNIX // MSVC

  // 'noexcept' operator for C++11
  #if _DREAMY_CPP11
    #define DREAMY_NOEXCEPT throw()
  #else
    #define DREAMY_NOEXCEPT
  #endif

  // Unix-styled 'errno_t' alias
  typedef int error_t;

#else // GCC and others

  // 'noexcept' operator for C++11
  #if _DREAMY_CPP11
    #define DREAMY_NOEXCEPT noexcept
  #else
    #define DREAMY_NOEXCEPT throw()
  #endif

  // Always inline method
  #define __forceinline __attribute__((always_inline)) inline

#endif

// Ignore assertions in non-debug mode
#if defined(NDEBUG) && !defined(_DREAMY_NDEBUG_ASSERT)
  // Assert about unexpected behavior if the expression check doesn't pass
  #define D_ASSERT(_Expression) ((void)0)

  // Assert about unexpected behavior every time
  #define D_WARNING(_Message) ((void)0)

#else
  #include <assert.h>

  #if _DREAMY_UNIX
    // Assert about unexpected behavior if the expression check doesn't pass
    #define D_ASSERT(_Expression) assert(_Expression)

    // Assert about unexpected behavior every time
    #define D_WARNING(_Message) assert(0 && _Message)

  #elif _DREAMY_CPP11
    #include <crtdbg.h>

    // Assert about unexpected behavior if the expression check doesn't pass
    #define D_ASSERT(_Expression) _ASSERT_EXPR(_Expression, _CRT_WIDE(#_Expression))

    // Assert about unexpected behavior every time
    #define D_WARNING(_Message) _ASSERT_EXPR(0, _Message)

  // C++98 under Windows
  #else
    // Assert about unexpected behavior if the expression check doesn't pass
    #define D_ASSERT(_Expression) do { if (!(_Expression)) { _assert(#_Expression, __FILE__, __LINE__); } } while (0)

    // Assert about unexpected behavior every time
    #define D_WARNING(_Message) do { _assert(_Message, __FILE__, __LINE__); } while (0)
  #endif

#endif

#include <cstring>
#include <cstdio>
#include <cstdarg>

// Secure method replacements for Unix and C++98
#if _DREAMY_UNIX || !_DREAMY_CPP11
  #define sscanf_s  sscanf
  #define swscanf_s swscanf
#endif

#if !_DREAMY_UNIX && !_DREAMY_CPP11
  #define vsnprintf _vsnprintf
  #define vswprintf _vsnwprintf
#endif

namespace dreamy {

// Scalar types
typedef unsigned char u8; // Byte
typedef   signed char s8;

typedef unsigned short u16;
typedef   signed short s16;

typedef unsigned int u32; // Same as 'size_t' on x86
typedef   signed int s32; // Same as 'int'

#if !_DREAMY_UNIX // MSVC 6.0 and above
  typedef unsigned __int64 u64; // Same as 'size_t' on x64
  typedef   signed __int64 s64;
#else
  typedef unsigned long long u64; // Same as 'size_t' on x64
  typedef   signed long long s64;
#endif

typedef float  f32; // Single precision
typedef double f64; // Double precision

// Text
typedef char c8; // ASCII character

// Maximum value of size_t
#define NULL_POS static_cast<size_t>(-1)

};

#endif // (Dreamy Utilities Include Guard)
