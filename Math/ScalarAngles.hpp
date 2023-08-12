//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_SCALARANGLES_H
#define _DREAMYUTILITIES_INCL_SCALARANGLES_H

#include "../Base/Base.hpp"

#include "../Math/Angles.hpp"
#include "../Math/ScalarVectors.hpp"

namespace dreamy {

#define NUMVEC_CLASS TAngles

// 32-bit integer angles
typedef TAngles<s32, 2> ang2i; // 2 integers
typedef TAngles<s32, 3> ang3i; // 3 integers
typedef TAngles<s32, 4> ang4i; // 4 integers

// Define arithmetic operators
#define VECTOR_TYPE s32
#define NUMVEC_OPS_FOR 2
#include "../Math/VectorOps/AnglesOps.inl"
#define NUMVEC_OPS_FOR 3
#include "../Math/VectorOps/AnglesOps.inl"
#define NUMVEC_OPS_FOR 4
#include "../Math/VectorOps/AnglesOps.inl"
#undef VECTOR_TYPE

// Single precision angles
typedef TAngles<f32, 2> ang2f; // 2D single precision angles
typedef TAngles<f32, 3> ang3f; // 3D single precision angles
typedef TAngles<f32, 4> ang4f; // 4D single precision angles

// Define arithmetic operators
#define VECTOR_TYPE f32
#define NUMVEC_OPS_FOR 2
#include "../Math/VectorOps/AnglesOps.inl"
#define NUMVEC_OPS_FOR 3
#include "../Math/VectorOps/AnglesOps.inl"
#define NUMVEC_OPS_FOR 4
#include "../Math/VectorOps/AnglesOps.inl"
#undef VECTOR_TYPE

// Double precision angles
typedef TAngles<f64, 2> ang2d; // 2D double precision angles
typedef TAngles<f64, 3> ang3d; // 3D double precision angles
typedef TAngles<f64, 4> ang4d; // 4D double precision angles

// Define arithmetic operators
#define VECTOR_TYPE f64
#define NUMVEC_OPS_FOR 2
#include "../Math/VectorOps/AnglesOps.inl"
#define NUMVEC_OPS_FOR 3
#include "../Math/VectorOps/AnglesOps.inl"
#define NUMVEC_OPS_FOR 4
#include "../Math/VectorOps/AnglesOps.inl"
#undef VECTOR_TYPE

#undef NUMVEC_CLASS

};

#endif // (Dreamy Utilities Include Guard)
