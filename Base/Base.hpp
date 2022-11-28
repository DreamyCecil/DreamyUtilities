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

// Platform-specific definitions
#include "PlatformSpecific.hpp"

// Other base components
#include "Types.hpp"
#include "Assert.hpp"
#include "Helper.hpp"

#endif // (Dreamy Utilities Include Guard)
