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

#ifndef _DREAMYUTILITIES_INCL_NUMERICVECTOR_H
#define _DREAMYUTILITIES_INCL_NUMERICVECTOR_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

#include "Math.hpp"

#include <cmath>

NAMESPACE_DREAMY_OPEN

// Vector type
#define NUMVEC TVector<Type, iDimensions>

// Set all dimensions in the vector to a certain value
#define NUMVEC_SET_ALL(VecValue) { \
  s32 i = iDimensions; \
  while (--i >= 0) m_aValues[i] = VecValue; \
}

// Fixed array of a certain number type
template<typename Type, const u32 iDimensions>
class TVector {

public:
  typedef Type T; // Template type

protected:
  Type m_aValues[iDimensions];

public:
  // Default constructor
  __forceinline TVector(void) {
    Clear();
  };

  // 1-value constructor
  __forceinline TVector(const Type val1) {
    m_aValues[0] = val1;
  };

  // 2-value constructor
  __forceinline TVector(const Type val1, const Type val2) {
    m_aValues[0] = val1; m_aValues[1] = val2;
  };

  // 3-value constructor
  __forceinline TVector(const Type val1, const Type val2, const Type val3) {
    m_aValues[0] = val1; m_aValues[1] = val2; m_aValues[2] = val3;
  };

  // 4-value constructor
  __forceinline TVector(const Type val1, const Type val2, const Type val3, const Type val4) {
    m_aValues[0] = val1; m_aValues[1] = val2; m_aValues[2] = val3; m_aValues[3] = val4;
  };

  // Array constructor
  __forceinline TVector(const Type *aValues) {
    NUMVEC_SET_ALL(aValues[i]);
  };

  // Copy constructor
  __forceinline TVector(const NUMVEC &vOther) {
    NUMVEC_SET_ALL(vOther[i]);
  };

  // Clear the vector
  __forceinline void Clear(void) {
    NUMVEC_SET_ALL(0);
  };

  // Get the value array
  __forceinline Type *Array(void) {
    return m_aValues;
  };

  // Get the value array (read-only)
  __forceinline const Type *Array(void) const {
    return m_aValues;
  };

  // Access value in the array
  __forceinline Type &operator[](s32 i) {
    return m_aValues[i];
  };

  // Access value in the array (read-only)
  __forceinline const Type &operator[](s32 i) const {
    return m_aValues[i];
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
      total = Type(total + m_aValues[i] * m_aValues[i]);
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
      if (m_aValues[i] != vOther[i]) {
        return false;
      }
    }
    return true;
  };

  // Check if vectors are different
  __forceinline bool operator!=(const NUMVEC &vOther) const {
    return !operator==(vOther);
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
template<typename Type, const u32 iDimensions>
inline NUMVEC NUMVEC::operator+(const NUMVEC &vOther) const {
  NUMVEC v(*this);

  s32 i = iDimensions;
  while (--i >= 0) {
    v[i] = Type(v[i] + vOther[i]);
  }
  return v;
};

template<typename Type, const u32 iDimensions>
inline NUMVEC &NUMVEC::operator+=(const NUMVEC &vOther) {
  (*this) = (*this) + vOther;
  return *this;
};

// Subtract another vector
template<typename Type, const u32 iDimensions>
inline NUMVEC NUMVEC::operator-(const NUMVEC &vOther) const {
  NUMVEC v(*this);

  s32 i = iDimensions;
  while (--i >= 0) {
    v[i] = Type(v[i] - vOther[i]);
  }
  return v;
};

template<typename Type, const u32 iDimensions>
inline NUMVEC &NUMVEC::operator-=(const NUMVEC &vOther) {
  (*this) = (*this) - vOther;
  return *this;
};

// Multiply by another vector
template<typename Type, const u32 iDimensions>
inline NUMVEC NUMVEC::operator*(const NUMVEC &vOther) const {
  NUMVEC v(*this);

  s32 i = iDimensions;
  while (--i >= 0) {
    v[i] = Type(v[i] * vOther[i]);
  }
  return v;
};

template<typename Type, const u32 iDimensions>
inline NUMVEC &NUMVEC::operator*=(const NUMVEC &vOther) {
  (*this) = (*this) * vOther;
  return *this;
};

// Divide by another vector
template<typename Type, const u32 iDimensions>
inline NUMVEC NUMVEC::operator/(const NUMVEC &vOther) const {
  NUMVEC v(*this);

  s32 i = iDimensions;
  while (--i >= 0) {
    v[i] = v[i] / vOther[i];
  }
  return v;
};

template<typename Type, const u32 iDimensions>
inline NUMVEC &NUMVEC::operator/=(const NUMVEC &vOther) {
  (*this) = (*this) / vOther;
  return *this;
};

// Negate the vector
template<typename Type, const u32 iDimensions>
inline NUMVEC NUMVEC::operator-(void) const {
  NUMVEC v(*this);

  s32 i = iDimensions;
  while (--i >= 0) {
    v[i] = -v[i];
  }
  return v;
};

// Dot product between two vectors
template<typename Type, const u32 iDimensions>
inline Type NUMVEC::operator%(const NUMVEC &vOther) const {
  Type product = 0;

  s32 i = iDimensions;
  while (--i >= 0) {
    product = Type(product + m_aValues[i] * vOther[i]);
  }

  return product;
};

// Multiply vector by a factor
template<typename Type, const u32 iDimensions>
inline NUMVEC NUMVEC::operator*(const Type factor) const {
  NUMVEC v(*this);

  s32 i = iDimensions;
  while (--i >= 0) {
    v[i] *= factor;
  }
  return v;
};

template<typename Type, const u32 iDimensions>
inline NUMVEC &NUMVEC::operator*=(const Type factor) {
  (*this) = (*this) * factor;
  return *this;
};

// Divide vector by a factor
template<typename Type, const u32 iDimensions>
inline NUMVEC NUMVEC::operator/(const Type factor) const {
  NUMVEC v(*this);

  s32 i = iDimensions;
  while (--i >= 0) {
    v[i] /= factor;
  }
  return v;
};

template<typename Type, const u32 iDimensions>
inline NUMVEC &NUMVEC::operator/=(const Type factor) {
  (*this) = (*this) / factor;
  return *this;
};

#undef NUMVEC

// 3D vector axis order
struct VecAxes {
  u8 x : 2;
  u8 y : 2;
  u8 z : 2;

  // Default constructor
  VecAxes() : x(0), y(1), z(2)
  {
  };

  // Order constructor
  VecAxes(u8 setX, u8 setY, u8 setZ)
  : x(setX & 3), y(setY & 3), z(setZ & 3)
  {
  };
};

#if !defined(_DREAMY_DEFAULT_AXES)
  // Default axes in 3D space are in a ZXY/201 order (with X being west/east and Y being up/down)
  #define _DREAMY_DEFAULT_AXES VecAxes(2, 0, 1)
#endif

NAMESPACE_DREAMY_CLOSE

#endif // (Dreamy Utilities Include Guard)
