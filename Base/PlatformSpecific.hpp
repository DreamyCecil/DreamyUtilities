//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_PLATFORM_SPECIFIC_H
#define _DREAMYUTILITIES_INCL_PLATFORM_SPECIFIC_H

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

#endif // (Dreamy Utilities Include Guard)
