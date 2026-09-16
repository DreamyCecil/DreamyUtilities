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

#ifndef _DREAMYUTILITIES_INCL_MATRIX12_H
#define _DREAMYUTILITIES_INCL_MATRIX12_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

#include "Matrix.hpp"

NAMESPACE_DREAMY_OPEN

// Rotation matrix with position
typedef TMatrix<f32, 3, 4> mat12f;
typedef TMatrix<f64, 3, 4> mat12d;

// Define operators
#define MATRIX_TYPE f32
#include "MatrixOps/Matrix12.inl"
#undef MATRIX_TYPE

#define MATRIX_TYPE f64
#include "MatrixOps/Matrix12.inl"
#undef MATRIX_TYPE

// Matrix template and types
#define ROTMAT3D TMatrix<Type, 3, 3>
#define ROTMAT12 TMatrix<Type, 3, 4>

// Make 4x3 matrix from a rotation matrix and a position vector
template<typename Type>
inline void Mat3DtoMat12(ROTMAT12 &m12,
  const ROTMAT3D &m3D, const TVector<Type, 3> &vPos)
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
  ROTMAT3D &m3D, TVector<Type, 3> &vPos)
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

NAMESPACE_DREAMY_CLOSE

#endif // (Dreamy Utilities Include Guard)
