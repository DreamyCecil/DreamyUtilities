//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_QVECTOR_H
#define _DREAMYUTILITIES_INCL_QVECTOR_H

#include "../Base/Base.hpp"

#include "../Math/Quaternion.hpp"
#include "../Math/Vector.hpp"
#include "../Math/Matrix12.hpp"

namespace dreamy {

// Placement in 3D space using quaternions for rotation
template<typename Type> class TQuatVec {

public:
  typedef Type T; // Template type

public:
  TNumVec<Type, 3> _pos;
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
typedef TQuatVec<f32> QuatVecF; // Single precision qvector
typedef TQuatVec<f64> QuatVecD; // Double precision qvector

};

#endif // (Dreamy Utilities Include Guard)
