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

#ifndef _DREAMYUTILITIES_INCL_MATH_RANDOM_H
#define _DREAMYUTILITIES_INCL_MATH_RANDOM_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

#include <cstdlib>

NAMESPACE_DREAMY_OPEN

// Get random number between 0 and 1 (inclusive)
inline f64 RandRatio(void) {
  return static_cast<f64>(rand()) / static_cast<f64>(RAND_MAX);
};

// Get random number between min and max (inclusive)
// Use more efficient "min + rand() % (max + 1)" for integers!
inline f64 RandRange(const f64 min, const f64 max) {
  return min + static_cast<f64>(rand()) / (static_cast<f64>(RAND_MAX) / (max - min));
};

NAMESPACE_DREAMY_CLOSE

#endif // (Dreamy Utilities Include Guard)
