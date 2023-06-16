//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_MATH_ALGEBRA_H
#define _DREAMYUTILITIES_INCL_MATH_ALGEBRA_H

#include "../Base/Base.hpp"

#include <math.h>

namespace dreamy {

namespace math {

// Math constants
static const f64 PI = 3.14159265359;

#define MATH_TEMP template<typename Type>

// Fast and type-safe sign function
MATH_TEMP inline s8 Sign(Type x) {
  return s8((Type(0) < x) - (x < Type(0)));
};

// Simple and compatible with C++98 copysign implementation
MATH_TEMP inline Type CopySign(Type x, Type y) {
  return Type(((x < 0 && y > 0) || (x > 0 && y < 0)) ? -x : x);
};

// Square root that accepts negative numbers
inline f64 SqrtSafe(f64 x) {
  // Slower alternative: 'sqrt(abs(x)) * Sign(x)'
  return (x >= 0) ? sqrt(x) : -sqrt(-x);
};

// Fast absolute value for unsigned 8-bit integer
inline u8 Abs(u8 x) {
  return x;
};

// Fast absolute value for unsigned 16-bit integer
inline u16 Abs(u16 x) {
  return x;
};

// Fast absolute value for unsigned 32-bit integer
inline u32 Abs(u32 x) {
  return x;
};

// Fast absolute value for unsigned 64-bit integer
inline u64 Abs(u64 x) {
  return x;
};

// Fast absolute value for signed 8-bit integer
inline s8 Abs(s8 x) {
  return x & 0x7F;
};

// Fast absolute value for signed 16-bit integer
inline s16 Abs(s16 x) {
  return x & 0x7FFF;
};

// Fast absolute value for signed 32-bit integer
inline s32 Abs(s32 x) {
  return x & 0x7FFFFFFF;
};

// Fast absolute value for signed 64-bit integer
inline s64 Abs(s64 x) {
  return x & 0x7FFFFFFFFFFFFFFF;
};

// Fast absolute value for single precision float
inline f32 Abs(f32 x) {
  s32 i = *(s32 *)&x;
  i &= 0x7FFFFFFF;
  return *(f32 *)&i;
};

// Fast absolute value for double precision float
inline f64 Abs(f64 x) {
  s64 i = *(s64 *)&x;
  i &= 0x7FFFFFFFFFFFFFFF;
  return *(f64 *)&i;
};

#undef MATH_TEMP

};

};

#endif // (Dreamy Utilities Include Guard)
