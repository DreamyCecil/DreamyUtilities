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

#ifndef _DREAMYUTILITIES_INCL_QVECTOR_H
#define _DREAMYUTILITIES_INCL_QVECTOR_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

#include "Quaternion.hpp"
#include "Vector.hpp"
#include "Matrix12.hpp"

NAMESPACE_DREAMY_OPEN

// Placement in 3D space using quaternions for rotation
template<typename Type>
class TQuatVec {

public:
  typedef Type T; // Template type

public:
  TVector<Type, 3> m_vPos;
  TQuaternion<Type> m_qRot;

public:
  // Convert from a 4x3 matrix (rotation with position)
  inline void FromMatrix12(const TMatrix<Type, 3, 4> &m12) {
    TMatrix<Type, 3, 3> m3D;

    Mat12toMat3D(m12, m3D, m_vPos);
    m_qRot.FromMatrix(m3D);
  };

  // Convert into a 4x3 matrix (rotation with position)
  inline void ToMatrix12(TMatrix<Type, 3, 4> &m12) {
    TMatrix<Type, 3, 3> m3D;

    m_qRot.ToMatrix(m3D);
    Mat3DtoMat12(m12, m3D, m_vPos);
  };
};

// Standard qvectors
typedef TQuatVec<f32> qvecf; // Single precision qvector
typedef TQuatVec<f64> qvecd; // Double precision qvector

NAMESPACE_DREAMY_CLOSE

#endif // (Dreamy Utilities Include Guard)
