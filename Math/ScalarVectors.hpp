//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_SCALARVECTORS_H
#define _DREAMYUTILITIES_INCL_SCALARVECTORS_H

#include "../DreamyUtilitiesBase.hpp"

#include "Vector.hpp"

#include <vector>

namespace dreamy {

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

}; // namespace dreamy

#endif // (Dreamy Utilities Include Guard)
