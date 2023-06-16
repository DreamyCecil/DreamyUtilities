//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_MATRIX12_H
#define _DREAMYUTILITIES_INCL_MATRIX12_H

#include "../Base/Base.hpp"

#include "RotationMatrix.hpp"

namespace dreamy {

// Rotation matrix with position
typedef TMatrix<f32, 3, 4> Mat12F;
typedef TMatrix<f64, 3, 4> Mat12D;

// Define operators
#define MATRIX_TYPE f32
#include "Operations/DefineOps12.inl"
#undef MATRIX_TYPE

#define MATRIX_TYPE f64
#include "Operations/DefineOps12.inl"
#undef MATRIX_TYPE

// Matrix template and types
#define ROTMAT3D TMatrix<Type, 3, 3>
#define ROTMAT12 TMatrix<Type, 3, 4>

// Make 4x3 matrix from a rotation matrix and a position vector
template<typename Type>
inline void Mat3DtoMat12(ROTMAT12 &m12,
  const ROTMAT3D &m3D, const TNumVec<Type, 3> &vPos)
{
  m12(0, 0) = m3D(0, 0);
  m12(0, 1) = m3D(0, 1);
  m12(0, 2) = m3D(0, 2);
  m12(1, 0) = m3D(1, 0);
  m12(1, 1) = m3D(1, 1);
  m12(1, 2) = m3D(1, 2);
  m12(2, 0) = m3D(2, 0);
  m12(2, 1) = m3D(2, 1);
  m12(2, 2) = m3D(2, 2);

  m12(0, 3) = vPos[0];
  m12(1, 3) = vPos[1];
  m12(2, 3) = vPos[2];
}

// Convert 4x3 matrix into a rotation matrix and a position vector
template<typename Type>
inline void Mat12toMat3D(const ROTMAT12 &m12,
  ROTMAT3D &m3D, TNumVec<Type, 3> &vPos)
{
  m3D(0, 0) = m12(0, 0);
  m3D(0, 1) = m12(0, 1);
  m3D(0, 2) = m12(0, 2);
  m3D(1, 0) = m12(1, 0);
  m3D(1, 1) = m12(1, 1);
  m3D(1, 2) = m12(1, 2);
  m3D(2, 0) = m12(2, 0);
  m3D(2, 1) = m12(2, 1);
  m3D(2, 2) = m12(2, 2);

  vPos[0] = m12(0, 3);
  vPos[1] = m12(1, 3);
  vPos[2] = m12(2, 3);
};

#undef ROTMAT3D
#undef ROTMAT12

};

#endif // (Dreamy Utilities Include Guard)
