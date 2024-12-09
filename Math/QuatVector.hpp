//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_QVECTOR_H
#define _DREAMYUTILITIES_INCL_QVECTOR_H

#include "../DreamyUtilitiesBase.hpp"

#include "Quaternion.hpp"
#include "Vector.hpp"
#include "Matrix12.hpp"

namespace dreamy {

// Placement in 3D space using quaternions for rotation
template<typename Type> class TQuatVec {

public:
  typedef Type T; // Template type

public:
  TVector<Type, 3> _pos;
  TQuaternion<Type> _rot;

public:
  // Convert from a 4x3 matrix (rotation with position)
  inline void FromMatrix12(const TMatrix<Type, 3, 4> &m12) {
    TMatrix<Type, 3, 3> m3D;

    Mat12toMat3D(m12, m3D, _pos);
    _rot.FromMatrix(m3D);
  };

  // Convert into a 4x3 matrix (rotation with position)
  inline void ToMatrix12(TMatrix<Type, 3, 4> &m12) {
    TMatrix<Type, 3, 3> m3D;

    _rot.ToMatrix(m3D);
    Mat3DtoMat12(m12, m3D, _pos);
  };
};

// Standard qvectors
typedef TQuatVec<f32> qvecf; // Single precision qvector
typedef TQuatVec<f64> qvecd; // Double precision qvector

}; // namespace dreamy

#endif // (Dreamy Utilities Include Guard)
