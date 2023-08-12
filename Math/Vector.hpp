//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_NUMERICVECTOR_H
#define _DREAMYUTILITIES_INCL_NUMERICVECTOR_H

#include "../Base/Base.hpp"

#include "../Math/Trigonometry.hpp"

#include <math.h>

namespace dreamy {

// Vector template and type
#define NUMVEC_TEMP template<typename Type, const u32 iDimensions>
#define NUMVEC TNumVec<Type, iDimensions>

// Set all dimensions in the vector to a certain value
#define NUMVEC_SET_ALL(VecValue) { \
  s32 i = iDimensions; \
  while (--i >= 0) _values[i] = VecValue; \
}

// Fixed array of a certain number type
NUMVEC_TEMP class TNumVec {

public:
  typedef Type T; // Template type

protected:
  Type _values[iDimensions];

public:
  // Default constructor
  __forceinline TNumVec(void) {
    Clear();
  };

  // 1-value constructor
  __forceinline TNumVec(const Type val1) {
    _values[0] = val1;
  };

  // 2-value constructor
  __forceinline TNumVec(const Type val1, const Type val2) {
    _values[0] = val1; _values[1] = val2;
  };

  // 3-value constructor
  __forceinline TNumVec(const Type val1, const Type val2, const Type val3) {
    _values[0] = val1; _values[1] = val2; _values[2] = val3;
  };

  // 4-value constructor
  __forceinline TNumVec(const Type val1, const Type val2, const Type val3, const Type val4) {
    _values[0] = val1; _values[1] = val2; _values[2] = val3; _values[3] = val4;
  };

  // Array constructor
  __forceinline TNumVec(const Type *aValues) {
    NUMVEC_SET_ALL(aValues[i]);
  };

  // Copy constructor
  __forceinline TNumVec(const NUMVEC &vOther) {
    NUMVEC_SET_ALL(vOther[i]);
  };

  // Clear the vector
  __forceinline void Clear(void) {
    NUMVEC_SET_ALL(0);
  };

  // Get the value array
  __forceinline Type *Array(void) {
    return _values;
  };

  // Get the value array (read-only)
  __forceinline const Type *Array(void) const {
    return _values;
  };

  // Access value in the array
  __forceinline Type &operator[](s32 i) {
    return _values[i];
  };

  // Access value in the array (read-only)
  __forceinline const Type &operator[](s32 i) const {
    return _values[i];
  };

  // Assignment from a value
  __forceinline NUMVEC &operator=(const Type value) {
    NUMVEC_SET_ALL(value);
    return *this;
  };

  // Assignment from another vector
  __forceinline NUMVEC &operator=(const NUMVEC &vOther) {
    if (&vOther == this) return *this;

    NUMVEC_SET_ALL(vOther[i]);
    return *this;
  };

  // Get length of the vector
  __forceinline Type Length(void) const {
    Type total = 0;

    s32 i = iDimensions;
    while (--i >= 0) {
      total = Type(total + _values[i] * _values[i]);
    }

    return (Type)sqrt(total);
  };

  // Normalize the vector
  __forceinline void Normalize(void) {
    (*this) /= Length();
  };

  // Normalize and return the vector
  __forceinline NUMVEC Normalize(void) const {
    return (*this) / Length();
  };

// Comparison
public:

  // Check if vectors are the same
  __forceinline bool operator==(const NUMVEC &vOther) const {
    s32 i = iDimensions;
    while (--i >= 0) {
      if (_values[i] != vOther[i]) {
        return false;
      }
    }
    return true;
  };

  // Check if vectors are different
  __forceinline bool operator!=(const NUMVEC &vOther) const {
    s32 i = iDimensions;
    while (--i >= 0) {
      if (_values[i] != vOther[i]) {
        return true;
      }
    }
    return false;
  };

// Arithmetic operations with another vector
public:

  // Add another vector
  NUMVEC operator+(const NUMVEC &vOther) const;
  NUMVEC &operator+=(const NUMVEC &vOther);

  // Subtract another vector
  NUMVEC operator-(const NUMVEC &vOther) const;
  NUMVEC &operator-=(const NUMVEC &vOther);

  // Multiply by another vector
  NUMVEC operator*(const NUMVEC &vOther) const;
  NUMVEC &operator*=(const NUMVEC &vOther);

  // Divide by another vector
  NUMVEC operator/(const NUMVEC &vOther) const;
  NUMVEC &operator/=(const NUMVEC &vOther);

  // Negate the vector
  NUMVEC operator-(void) const;

  // Dot product between two vectors
  Type operator%(const NUMVEC &vOther) const;

// Arithmetic operations with a factor
public:

  // Multiply vector by a factor
  NUMVEC operator*(const Type factor) const;
  NUMVEC &operator*=(const Type factor);

  // Divide vector by a factor
  NUMVEC operator/(const Type factor) const;
  NUMVEC &operator/=(const Type factor);
};

// Add another vector
NUMVEC_TEMP inline
NUMVEC NUMVEC::operator+(const NUMVEC &vOther) const {
  NUMVEC v(*this);

  s32 i = iDimensions;
  while (--i >= 0) {
    v[i] = Type(v[i] + vOther[i]);
  }
  return v;
};

NUMVEC_TEMP inline
NUMVEC &NUMVEC::operator+=(const NUMVEC &vOther) {
  (*this) = (*this) + vOther;
  return *this;
};

// Subtract another vector
NUMVEC_TEMP inline
NUMVEC NUMVEC::operator-(const NUMVEC &vOther) const {
  NUMVEC v(*this);

  s32 i = iDimensions;
  while (--i >= 0) {
    v[i] = Type(v[i] - vOther[i]);
  }
  return v;
};

NUMVEC_TEMP inline
NUMVEC &NUMVEC::operator-=(const NUMVEC &vOther) {
  (*this) = (*this) - vOther;
  return *this;
};

// Multiply by another vector
NUMVEC_TEMP inline
NUMVEC NUMVEC::operator*(const NUMVEC &vOther) const {
  NUMVEC v(*this);

  s32 i = iDimensions;
  while (--i >= 0) {
    v[i] = Type(v[i] * vOther[i]);
  }
  return v;
};

NUMVEC_TEMP inline
NUMVEC &NUMVEC::operator*=(const NUMVEC &vOther) {
  (*this) = (*this) * vOther;
  return *this;
};

// Divide by another vector
NUMVEC_TEMP inline
NUMVEC NUMVEC::operator/(const NUMVEC &vOther) const {
  NUMVEC v(*this);

  s32 i = iDimensions;
  while (--i >= 0) {
    v[i] = v[i] / vOther[i];
  }
  return v;
};

NUMVEC_TEMP inline
NUMVEC &NUMVEC::operator/=(const NUMVEC &vOther) {
  (*this) = (*this) / vOther;
  return *this;
};

// Negate the vector
NUMVEC_TEMP inline
NUMVEC NUMVEC::operator-(void) const {
  NUMVEC v(*this);

  s32 i = iDimensions;
  while (--i >= 0) {
    v[i] = -v[i];
  }
  return v;
};

// Dot product between two vectors
NUMVEC_TEMP inline
Type NUMVEC::operator%(const NUMVEC &vOther) const {
  Type product = 0;

  s32 i = iDimensions;
  while (--i >= 0) {
    product = Type(product + _values[i] * vOther[i]);
  }

  return product;
};

// Multiply vector by a factor
NUMVEC_TEMP inline
NUMVEC NUMVEC::operator*(const Type factor) const {
  NUMVEC v(*this);

  s32 i = iDimensions;
  while (--i >= 0) {
    v[i] *= factor;
  }
  return v;
};

NUMVEC_TEMP inline
NUMVEC &NUMVEC::operator*=(const Type factor) {
  (*this) = (*this) * factor;
  return *this;
};

// Divide vector by a factor
NUMVEC_TEMP inline
NUMVEC NUMVEC::operator/(const Type factor) const {
  NUMVEC v(*this);

  s32 i = iDimensions;
  while (--i >= 0) {
    v[i] /= factor;
  }
  return v;
};

NUMVEC_TEMP inline
NUMVEC &NUMVEC::operator/=(const Type factor) {
  (*this) = (*this) / factor;
  return *this;
};

#undef NUMVEC
#undef NUMVEC_TEMP

// 3D vector axis order
struct VecAxes {
  u8 _x : 2;
  u8 _y : 2;
  u8 _z : 2;

  // Default constructor
  VecAxes() : _x(0), _y(1), _z(2)
  {
  };

  // Order constructor
  VecAxes(u8 setX, u8 setY, u8 setZ)
  : _x(setX & 3), _y(setY & 3), _z(setZ & 3)
  {
  };
};

#if !defined(_DREAMY_DEFAULT_AXES)
  // Default axes in 3D space are in a ZXY/201 order (with X being west/east and Y being up/down)
  #define _DREAMY_DEFAULT_AXES VecAxes(2, 0, 1)
#endif

};

#endif // (Dreamy Utilities Include Guard)
