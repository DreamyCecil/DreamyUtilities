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

#ifndef _DREAMYUTILITIES_INCL_MATH_TRIGONOMETRY_H
#define _DREAMYUTILITIES_INCL_MATH_TRIGONOMETRY_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

#include "Algebra.hpp"

NAMESPACE_DREAMY_OPEN

// Wrap angle to be between 0 and 360 degrees
template<typename Type>
inline Type WrapAngle(Type angle) {
  return (Type)fmod(fmod((f64)angle, 360.0) + 360.0, 360.0);
};

// Normalize angle to be between -180 and +180 degrees
template<typename Type>
inline Type NormalizeAngle(Type angle) {
  return Type(WrapAngle((f64)angle + 180.0) - 180.0);
};

// Convert radians to degrees
template<typename Type>
inline Type RadToDeg(Type radians) {
  return Type((f64)radians * (f64(180.0) / dreamy::math::PI));
};

// Convert degrees to radians
template<typename Type>
inline Type DegToRad(Type angle) {
  return Type(WrapAngle((f64)angle) * (dreamy::math::PI / 180.0));
};

NAMESPACE_DREAMY_CLOSE

#endif // (Dreamy Utilities Include Guard)
