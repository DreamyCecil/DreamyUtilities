//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_TYPES_H
#define _DREAMYUTILITIES_INCL_TYPES_H

#include <string>

namespace dreamy
{
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
  typedef std::string Str_t; // ASCII string

  // Maximum value of size_t
  #define NULL_POS static_cast<size_t>(-1)

};

#endif // (Dreamy Utilities Include Guard)
