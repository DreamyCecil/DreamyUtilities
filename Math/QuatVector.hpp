//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

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
