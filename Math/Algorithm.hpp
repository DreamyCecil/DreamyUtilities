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

#ifndef _DREAMYUTILITIES_INCL_MATH_ALGORITHM_H
#define _DREAMYUTILITIES_INCL_MATH_ALGORITHM_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

NAMESPACE_DREAMY_OPEN

namespace math {

// Return smaller of two values
template<typename Type1, typename Type2>
__forceinline Type1 Min(Type1 x, Type2 y) {
  return (x < y) ? x : y;
};

// Return larger of two values
template<typename Type1, typename Type2>
__forceinline Type1 Max(Type1 x, Type2 y) {
  return (x < y) ? y : x;
};

// Clamp value between certain boundaries
template<typename Type1, typename Type2, typename Type3>
__forceinline Type1 Clamp(Type1 x, Type2 min, Type3 max) {
  return (x < min) ? min : (max < x) ? max : x;
};

}; // namespace math

NAMESPACE_DREAMY_CLOSE

#endif // (Dreamy Utilities Include Guard)
