//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_ANGLES_H
#define _DREAMYUTILITIES_INCL_ANGLES_H

#include "../Base/Base.hpp"

#include "../Math/Matrix.hpp"
#include "../Math/Vector.hpp"
#include "../Math/Trigonometry.hpp"

namespace dreamy {

// Vector template and type
#define ANGLES_TEMP template<typename Type, const u32 iDimensions>
#define ANGLES TAngles<Type, iDimensions>

// Fixed array of rotation angles
ANGLES_TEMP
class TAngles : public TNumVec<Type, iDimensions> {

public:
  // Default constructor
  __forceinline TAngles(void) {
    this->Clear();
  };

  // 1-value constructor
  __forceinline TAngles(const Type val1) {
    this->_values[0] = val1;
  };

  // 2-value constructor
  __forceinline TAngles(const Type val1, const Type val2) {
    this->_values[0] = val1; this->_values[1] = val2;
  };

  // 3-value constructor
  __forceinline TAngles(const Type val1, const Type val2, const Type val3) {
    this->_values[0] = val1; this->_values[1] = val2; this->_values[2] = val3;
  };

  // 4-value constructor
  __forceinline TAngles(const Type val1, const Type val2, const Type val3, const Type val4) {
    this->_values[0] = val1; this->_values[1] = val2; this->_values[2] = val3; this->_values[3] = val4;
  };

  // 5-value constructor
  __forceinline TAngles(const Type val1, const Type val2, const Type val3, const Type val4, const Type val5) {
    this->_values[0] = val1; this->_values[1] = val2; this->_values[2] = val3;
    this->_values[3] = val4; this->_values[4] = val5;
  };

  // 6-value constructor
  __forceinline TAngles(const Type val1, const Type val2, const Type val3, const Type val4, const Type val5, const Type val6) {
    this->_values[0] = val1; this->_values[1] = val2; this->_values[2] = val3;
    this->_values[3] = val4; this->_values[4] = val5; this->_values[5] = val6;
  };

public:
  // Calculate direction vector from euler angles
  inline void AnglesToDir(TNumVec<Type, 3> &vDirection) const {
    // Make rotation matrix from the angles
    TMatrix<Type, 3, 3> mDirection;
    Mat3DFromAngles(mDirection, *this);

    // Rotate a front-facing vector by the matrix
    vDirection = TNumVec<Type, 3>((Type)0.0, (Type)0.0, (Type)-1.0) * mDirection;
  };

  // Calculate euler angles from a direction vector (without banking)
  inline void DirToAngles(TNumVec<Type, 3> &vAngles) const {
    Type &h = vAngles[0];
    Type &p = vAngles[1];
    Type &b = vAngles[2];

    const Type &x = (*this)[0];
    const Type &y = (*this)[1];
    const Type &z = (*this)[2];

    b = 0; // Banking is irrelevant
    p = (Type)asin(y); // Calculate pitch

    // If pointing upwards or downwards
    if ((f64)y > 0.99 || (f64)y < -0.99) {
      // Heading is irrelevant
      h = 0;

    } else {
      // Calculate heading
      h = (Type)atan2(-x, -z);
    }
  };

  // Rotate euler angles in radians using the trackball method
  void RotateTrackball(const TNumVec<Type, 3> &vRotation) {
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
  void RotateAirplane(const TNumVec<Type, 3> &vRotation) {
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
ANGLES_TEMP inline
ANGLES ANGLES::DegToRad(void) const {
  ANGLES v(*this);

  s32 i = iDimensions;
  while (--i >= 0) {
    v[i] = dreamy::DegToRad(this->_values[i]);
  }
  return v;
};

// Convert values from radians to degrees
ANGLES_TEMP inline
ANGLES ANGLES::RadToDeg(void) const {
  ANGLES v(*this);

  s32 i = iDimensions;
  while (--i >= 0) {
    v[i] = dreamy::RadToDeg(this->_values[i]);
  }
  return v;
};

#undef ANGLES
#undef ANGLES_TEMP

};

#endif // (Dreamy Utilities Include Guard)
