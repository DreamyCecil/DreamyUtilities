//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_SCALARVECTORS_H
#define _DREAMYUTILITIES_INCL_SCALARVECTORS_H

#include "../Base/Base.hpp"

#include "../Math/Vector.hpp"

#include <vector>

namespace dreamy {

#define NUMVEC_CLASS TNumVec

// 32-bit integer vectors
typedef TNumVec<s32, 2> Vec2I; // 2 integers
typedef TNumVec<s32, 3> Vec3I; // 3 integers
typedef TNumVec<s32, 4> Vec4I; // 4 integers

// Define arithmetic operators
#define VECTOR_TYPE s32
#define NUMVEC_OPS_FOR 2
#include "../Math/VectorOps/VectorOps.inl"
#define NUMVEC_OPS_FOR 3
#include "../Math/VectorOps/VectorOps.inl"
#define NUMVEC_OPS_FOR 4
#include "../Math/VectorOps/VectorOps.inl"
#undef VECTOR_TYPE

// Single precision vectors
typedef TNumVec<f32, 2> Vec2F; // 2D single precision vector
typedef TNumVec<f32, 3> Vec3F; // 3D single precision vector
typedef TNumVec<f32, 4> Vec4F; // 4D single precision vector

// Define arithmetic operators
#define VECTOR_TYPE f32
#define NUMVEC_OPS_FOR 2
#include "../Math/VectorOps/VectorOps.inl"
#define NUMVEC_OPS_FOR 3
#include "../Math/VectorOps/VectorOps.inl"
#define NUMVEC_OPS_FOR 4
#include "../Math/VectorOps/VectorOps.inl"
#undef VECTOR_TYPE

// Double precision vectors
typedef TNumVec<f64, 2> Vec2D; // 2D double precision vector
typedef TNumVec<f64, 3> Vec3D; // 3D double precision vector
typedef TNumVec<f64, 4> Vec4D; // 4D double precision vector

// Define arithmetic operators
#define VECTOR_TYPE f64
#define NUMVEC_OPS_FOR 2
#include "../Math/VectorOps/VectorOps.inl"
#define NUMVEC_OPS_FOR 3
#include "../Math/VectorOps/VectorOps.inl"
#define NUMVEC_OPS_FOR 4
#include "../Math/VectorOps/VectorOps.inl"
#undef VECTOR_TYPE

#undef NUMVEC_CLASS

// Array types
typedef std::vector<Vec2D> CVec2Array; // Array of 2D vectors
typedef std::vector<Vec3D> CVec3Array; // Array of 3D vectors

};

#endif // (Dreamy Utilities Include Guard)
