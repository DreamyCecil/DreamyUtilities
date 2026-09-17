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

#ifndef _DREAMYUTILITIES_INCL_ANGLES_H
#define _DREAMYUTILITIES_INCL_ANGLES_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

#include "Math.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

NAMESPACE_DREAMY_OPEN

// Vector type
#define ANGLES TAngles<Type, iDimensions>

// Fixed array of rotation angles
template<typename Type, const u32 iDimensions>
class TAngles : public TVector<Type, iDimensions> {

public:
  // Default constructor
  __forceinline TAngles(void) {
    this->Clear();
  };

  // 1-value constructor
  __forceinline TAngles(const Type val1) {
    this->m_aValues[0] = val1;
  };

  // 2-value constructor
  __forceinline TAngles(const Type val1, const Type val2) {
    this->m_aValues[0] = val1; this->m_aValues[1] = val2;
  };

  // 3-value constructor
  __forceinline TAngles(const Type val1, const Type val2, const Type val3) {
    this->m_aValues[0] = val1; this->m_aValues[1] = val2; this->m_aValues[2] = val3;
  };

  // 4-value constructor
  __forceinline TAngles(const Type val1, const Type val2, const Type val3, const Type val4) {
    this->m_aValues[0] = val1; this->m_aValues[1] = val2; this->m_aValues[2] = val3; this->m_aValues[3] = val4;
  };

  // 5-value constructor
  __forceinline TAngles(const Type val1, const Type val2, const Type val3, const Type val4, const Type val5) {
    this->m_aValues[0] = val1; this->m_aValues[1] = val2; this->m_aValues[2] = val3;
    this->m_aValues[3] = val4; this->m_aValues[4] = val5;
  };

  // 6-value constructor
  __forceinline TAngles(const Type val1, const Type val2, const Type val3, const Type val4, const Type val5, const Type val6) {
    this->m_aValues[0] = val1; this->m_aValues[1] = val2; this->m_aValues[2] = val3;
    this->m_aValues[3] = val4; this->m_aValues[4] = val5; this->m_aValues[5] = val6;
  };

public:
  // Calculate direction vector from euler angles
  inline void AnglesToDir(TVector<Type, 3> &vDirection) const {
    // Make rotation matrix from the angles
    TMatrix<Type, 3, 3> mDirection;
    Mat3DFromAngles(mDirection, *this);

    // Rotate a front-facing vector by the matrix
    vDirection = TVector<Type, 3>((Type)0.0, (Type)0.0, (Type)-1.0) * mDirection;
  };

  // Calculate euler angles from a direction vector (without banking)
  inline void DirToAngles(TVector<Type, 3> &vAngles) const {
    Type &h = vAngles[0];
    Type &p = vAngles[1];
    Type &b = vAngles[2];

    const Type &x = (*this)[0];
    const Type &y = (*this)[1];
    const Type &z = (*this)[2];

    b = 0; // Banking is irrelevant
    p = (Type)asin(y); // Calculate pitch

    // If pointing upwards or downwards
    if (dreamy::math::Abs((f64)y) >= 1.0 - 1e-6) {
      // Heading is irrelevant
      h = 0;

    } else {
      // Calculate heading
      h = (Type)atan2(-x, -z);
    }
  };

  // Rotate euler angles in radians using the trackball method
  void RotateTrackball(const TVector<Type, 3> &vRotation) {
    TMatrix<Type, 3, 3> mRotation;
    TMatrix<Type, 3, 3> mOriginal;

    // Create matrices from angles
    Mat3DFromAngles(mRotation, vRotation);
    Mat3DFromAngles(mOriginal, *this);

    // Recreate angles from the composed matrix
    mOriginal = mRotation * mOriginal; // Rotate first by original, then by rotation angles
    Mat3DToAngles(mOriginal, *this);
  };

  // Rotate euler angles in radians using the airplane method
  void RotateAirplane(const TVector<Type, 3> &vRotation) {
    TMatrix<Type, 3, 3> mRotation;
    TMatrix<Type, 3, 3> mOriginal;

    // Create matrices from angles
    Mat3DFromAngles(mRotation, vRotation);
    Mat3DFromAngles(mOriginal, *this);

    // Recreate angles from the composed matrix
    mOriginal = mOriginal * mRotation; // Rotate first by rotation, then by original angles
    Mat3DToAngles(mOriginal, *this);
  };

  // Convert values from degrees to radians
  ANGLES DegToRad(void) const;

  // Convert values from radians to degrees
  ANGLES RadToDeg(void) const;
};

// Convert values from degrees to radians
template<typename Type, const u32 iDimensions>
inline ANGLES ANGLES::DegToRad(void) const {
  ANGLES v(*this);

  s32 i = iDimensions;
  while (--i >= 0) {
    v[i] = dreamy::math::DegToRad(this->m_aValues[i]);
  }
  return v;
};

// Convert values from radians to degrees
template<typename Type, const u32 iDimensions>
inline ANGLES ANGLES::RadToDeg(void) const {
  ANGLES v(*this);

  s32 i = iDimensions;
  while (--i >= 0) {
    v[i] = dreamy::math::RadToDeg(this->m_aValues[i]);
  }
  return v;
};

#undef ANGLES

NAMESPACE_DREAMY_CLOSE

#endif // (Dreamy Utilities Include Guard)
