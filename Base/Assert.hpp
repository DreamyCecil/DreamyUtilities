//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_ASSERT_H
#define _DREAMYUTILITIES_INCL_ASSERT_H

#include "PlatformSpecific.hpp"

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

  #else
    // Assert about unexpected behavior if the expression check doesn't pass
    #define D_ASSERT(_Expression) _ASSERT_EXPR(_Expression, _CRT_WIDE(#_Expression))

    // Assert about unexpected behavior every time
    #define D_WARNING(_Message) _ASSERT_EXPR(0, _Message)
  #endif

#endif

#endif // (Dreamy Utilities Include Guard)
