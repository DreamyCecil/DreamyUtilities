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

#ifndef _DREAMYUTILITIES_INCL_SCALARVECTORS_H
#define _DREAMYUTILITIES_INCL_SCALARVECTORS_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

#include "Vector.hpp"

#include <vector>

NAMESPACE_DREAMY_OPEN

#define NUMVEC_CLASS TVector

// 32-bit integer vectors
typedef TVector<s32, 2> vec2i; // 2 integers
typedef TVector<s32, 3> vec3i; // 3 integers
typedef TVector<s32, 4> vec4i; // 4 integers

// Define arithmetic operators
#define VECTOR_TYPE s32
#define NUMVEC_OPS_FOR 2
#include "VectorOps/VectorOps.inl"
#define NUMVEC_OPS_FOR 3
#include "VectorOps/VectorOps.inl"
#define NUMVEC_OPS_FOR 4
#include "VectorOps/VectorOps.inl"
#undef VECTOR_TYPE

// Single precision vectors
typedef TVector<f32, 2> vec2f; // 2D single precision vector
typedef TVector<f32, 3> vec3f; // 3D single precision vector
typedef TVector<f32, 4> vec4f; // 4D single precision vector

// Define arithmetic operators
#define VECTOR_TYPE f32
#define NUMVEC_OPS_FOR 2
#include "VectorOps/VectorOps.inl"
#define NUMVEC_OPS_FOR 3
#include "VectorOps/VectorOps.inl"
#define NUMVEC_OPS_FOR 4
#include "VectorOps/VectorOps.inl"
#undef VECTOR_TYPE

// Double precision vectors
typedef TVector<f64, 2> vec2d; // 2D double precision vector
typedef TVector<f64, 3> vec3d; // 3D double precision vector
typedef TVector<f64, 4> vec4d; // 4D double precision vector

// Define arithmetic operators
#define VECTOR_TYPE f64
#define NUMVEC_OPS_FOR 2
#include "VectorOps/VectorOps.inl"
#define NUMVEC_OPS_FOR 3
#include "VectorOps/VectorOps.inl"
#define NUMVEC_OPS_FOR 4
#include "VectorOps/VectorOps.inl"
#undef VECTOR_TYPE

#undef NUMVEC_CLASS

// Array types
typedef std::vector<vec2d> Vec2Array_t; // Array of 2D vectors
typedef std::vector<vec3d> Vec3Array_t; // Array of 3D vectors

NAMESPACE_DREAMY_CLOSE

#endif // (Dreamy Utilities Include Guard)
