/* Copyright (c) 2022-2026 Dreamy Cecil
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DREAMYUTILITIES_INCL_MATH_ALGEBRA_H
#define _DREAMYUTILITIES_INCL_MATH_ALGEBRA_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

#include <cfloat>
#include <cmath>

NAMESPACE_DREAMY_OPEN

namespace math {

// Math constants
static const f64 PI = 3.14159265359;

static const f64 Infinity = HUGE_VAL;
static const f64 NaN = Infinity * 0.0;

// Fast and type-safe sign function
template<typename Type>
__forceinline s8 Sign(Type x) {
  return s8((Type(0) < x) - (x < Type(0)));
};

// Simple and compatible with C++98 copysign implementation
template<typename Type>
__forceinline Type CopySign(Type x, Type y) {
  return Type(((x < 0 && y > 0) || (x > 0 && y < 0)) ? -x : x);
};

// Square root that accepts negative numbers
__forceinline f64 SqrtSign(f64 x) {
  // Slower alternative: 'sqrt(abs(x)) * Sign(x)'
  return (x >= 0) ? sqrt(x) : -sqrt(-x);
};

// Fast absolute value for unsigned 8-bit integer
__forceinline u8 Abs(u8 x) {
  return x;
};

// Fast absolute value for unsigned 16-bit integer
__forceinline u16 Abs(u16 x) {
  return x;
};

// Fast absolute value for unsigned 32-bit integer
__forceinline u32 Abs(u32 x) {
  return x;
};

// Fast absolute value for unsigned 64-bit integer
__forceinline u64 Abs(u64 x) {
  return x;
};

// Fast absolute value for signed 8-bit integer
__forceinline s8 Abs(s8 x) {
  const s8 iMask = x >> 7;
  return (x + iMask) ^ iMask;
};

// Fast absolute value for signed 16-bit integer
__forceinline s16 Abs(s16 x) {
  const s16 iMask = x >> 15;
  return (x + iMask) ^ iMask;
};

// Fast absolute value for signed 32-bit integer
__forceinline s32 Abs(s32 x) {
  const s32 iMask = x >> 31;
  return (x + iMask) ^ iMask;
};

// Fast absolute value for signed 64-bit integer
__forceinline s64 Abs(s64 x) {
  const s64 iMask = x >> 63;
  return (x + iMask) ^ iMask;
};

// Fast absolute value for single precision float
__forceinline f32 Abs(f32 x) {
  s32 i = *reinterpret_cast<s32 *>(&x);
  i &= 0x7FFFFFFF;
  return *reinterpret_cast<f32 *>(&i);
};

// Fast absolute value for double precision float
__forceinline f64 Abs(f64 x) {
  s64 i = *reinterpret_cast<s64 *>(&x);
  i &= 0x7FFFFFFFFFFFFFFF;
  return *reinterpret_cast<f64 *>(&i);
};

// Check if float is not a number
__forceinline bool IsNaN(f64 x) {
  #if !_DREAMY_UNIX
    return _isnan(x) != 0;
  #else
    return __builtin_isnan(x) != 0;
  #endif
};

// Return sign of infinite float or 0 if it's not infinity
__forceinline s8 InfinitySign(f64 x) {
  if (x == Infinity) {
    return +1;
  } else if (x == -Infinity) {
    return -1;
  } else {
    return 0;
  }
};

}; // namespace math

NAMESPACE_DREAMY_CLOSE

#endif // (Dreamy Utilities Include Guard)
