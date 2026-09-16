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

#ifndef _DREAMYUTILITIES_INCL_QUATERNION_H
#define _DREAMYUTILITIES_INCL_QUATERNION_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

#include "Vector.hpp"
#include "ScalarMatrices.hpp"

NAMESPACE_DREAMY_OPEN

#define QUAT TQuaternion<Type>

// Quaternion of a certain type
template<typename Type>
class TQuaternion {

public:
  typedef Type T; // Template type
  typedef TVector<Type, 4> Vector;

public:
  union {
    Type m_aValues[4];
    struct { Type m_w, m_x, m_y, m_z; };
  };

public:
  // Default constructor
  __forceinline TQuaternion(void)
  {
  };

  // Value constructor
  __forceinline TQuaternion(Type wSet, Type xSet, Type ySet, Type zSet) :
    m_w(wSet), m_x(xSet), m_y(ySet), m_z(zSet)
  {
  };

  // Vector constructor
  __forceinline TQuaternion(const Vector &v) :
    m_w(v[0]), m_x(v[1]), m_y(v[2]), m_z(v[3])
  {
  };

  // Convert into a vector
  __forceinline Vector ToVector(void) const {
    return Vector(m_w, m_x, m_y, m_z);
  };

  // Convert from euler angles (H, P, B) in radians
  void FromEuler(const TVector<Type, 3> &vAngles, const VecAxes axes = _DREAMY_DEFAULT_AXES) {
    Type cosH = (Type)cos(vAngles[0] * Type(0.5));
    Type sinH = (Type)sin(vAngles[0] * Type(0.5));
    Type cosP = (Type)cos(vAngles[1] * Type(0.5));
    Type sinP = (Type)sin(vAngles[1] * Type(0.5));
    Type cosB = (Type)cos(vAngles[2] * Type(0.5));
    Type sinB = (Type)sin(vAngles[2] * Type(0.5));

    m_w = cosB * cosP * cosH + sinB * sinP * sinH;

    // From Wikipedia, offsetted for 3D space with upward Z axis
    //m_x = sinB * cosP * cosH - cosB * sinP * sinH;
    //m_y = cosB * sinP * cosH + sinB * cosP * sinH;
    //m_z = cosB * cosP * sinH - sinB * sinP * cosH;

    // ZXY/201 order by default for 3D space with upward Y axis
    (&m_x)[axes.x] = sinB * cosP * cosH - cosB * sinP * sinH;
    (&m_x)[axes.y] = cosB * sinP * cosH + sinB * cosP * sinH;
    (&m_x)[axes.z] = cosB * cosP * sinH - sinB * sinP * cosH;
  };

  // Convert to euler angles (H, P, B) in radians
  void ToEuler(TVector<Type, 3> &vAngles, const VecAxes axes = _DREAMY_DEFAULT_AXES) {
    // From Wikipedia, offsetted for 3D space with upward Z axis
    //Type axisValue[3] = { m_x, m_y, m_z };

    // ZXY/201 order by default for 3D space with upward Y axis
    Type axisValue[3] = { (&m_x)[axes.x], (&m_x)[axes.y], (&m_x)[axes.z] };

    // Heading
    Type sinH_cosP = 2 * (m_w * axisValue[2] + axisValue[0] * axisValue[1]);
    Type cosH_cosP = 1 - 2 * (axisValue[1] * axisValue[1] + axisValue[2] * axisValue[2]);
    vAngles[0] = (Type)atan2(sinH_cosP, cosH_cosP);

    // Pitch
    Type sinP = 2 * (m_w * axisValue[1] - axisValue[2] * axisValue[0]);

    if (dreamy::math::Abs(sinP) >= 1) {
      vAngles[1] = dreamy::math::CopySign(Type(dreamy::math::PI / 2), sinP); // Use 90 degrees if out of range
    } else {
      vAngles[1] = (Type)asin(sinP);
    }

    // Banking
    Type sinB_cosP = 2 * (m_w * axisValue[0] + axisValue[1] * axisValue[2]);
    Type cosB_cosP = 1 - 2 * (axisValue[0] * axisValue[0] + axisValue[1] * axisValue[1]);
    vAngles[2] = (Type)atan2(sinB_cosP, cosB_cosP);
  };

  // Convert to a rotation matrix
  void ToMatrix(TMatrix<Type, 3, 3> &matrix) const {
    Type wx = 2 * m_w * m_x;
    Type wy = 2 * m_w * m_y;
    Type wz = 2 * m_w * m_z;

    Type xx = 2 * m_x * m_x;
    Type xy = 2 * m_x * m_y;
    Type xz = 2 * m_x * m_z;

    Type yy = 2 * m_y * m_y;
    Type yz = 2 * m_y * m_z;
    Type zz = 2 * m_z * m_z;

    matrix(0, 0) = Type(1.0) - (yy + zz);
    matrix(0, 1) = xy - wz;
    matrix(0, 2) = xz + wy;

    matrix(1, 0) = xy + wz;
    matrix(1, 1) = Type(1.0) - (xx + zz);
    matrix(1, 2) = yz - wx;

    matrix(2, 0) = xz - wy;
    matrix(2, 1) = yz + wx;
    matrix(2, 2) = Type(1.0) - (xx + yy);
  };

  // Convert from a rotation matrix
  void FromMatrix(TMatrix<Type, 3, 3> &matrix) {
    Type trace = matrix(0, 0) + matrix(1, 1) + matrix(2, 2);
    Type root;

    if (trace > 0.0) {
      // abs(w) > 1/2 or w > 1/2
      root = (Type)sqrt(trace + Type(1.0)); // 2w
      m_w = Type(0.5) * root;
      root = Type(0.5) / root; // 1/(4w)

      // ZXY/201 order by default for 3D space with upward Y axis
      m_x = (matrix(2, 1) - matrix(1, 2)) * root;
      m_y = (matrix(0, 2) - matrix(2, 0)) * root;
      m_z = (matrix(1, 0) - matrix(0, 1)) * root;

    } else {
      // abs(w) <= 1/2
      static u32 aNext[3] = { 1, 2, 0 };
      u32 i = 0;

      if (matrix(1, 1) > matrix(0, 0)) {
        i = 1;
      }

      if (matrix(2, 2) > matrix(i, i)) {
        i = 2;
      }

      u32 j = aNext[i];
      u32 k = aNext[j];

      root = (Type)sqrt(matrix(i, i) - matrix(j, j) - matrix(k, k) + Type(1.0));

      Type *quat[3] = { &m_x, &m_y, &m_z };
      *quat[i] = Type(0.5) * root;

      root = Type(0.5) / root;
      m_w = (matrix(k, j) - matrix(j, k)) * root;

      *quat[j] = (matrix(j, i) + matrix(i, j)) * root;
      *quat[k] = (matrix(k, i) + matrix(i, k)) * root;
    }
  };

  // Convert from axis angle (in radians)
  void FromAxisAngle(const TVector<Type, 3> &vAngles, const Type angle, const VecAxes axes = VecAxes()) {
    Type sinAngle = (Type)sin(angle / 2);

    m_w = (Type)cos(angle / 2);
    m_x = vAngles[axes.x] * sinAngle;
    m_y = vAngles[axes.y] * sinAngle;
    m_z = vAngles[axes.z] * sinAngle;
  };

  // Convert to axis angle (in radians)
  void ToAxisAngle(TVector<Type, 3> &vAngles, Type &angle, const VecAxes axes = VecAxes()) {
    Type sinAngle = (Type)sqrt(Type(1) - m_w * m_w);
    angle = 2 * (Type)acos(m_w);

    // Angle isn't zero
    if (dreamy::math::Abs(sinAngle) >= 0.001) {
      vAngles[axes.x] = m_x / sinAngle;
      vAngles[axes.y] = m_y / sinAngle;
      vAngles[axes.z] = m_z / sinAngle;

    // Angle is zero
    } else {
      vAngles[axes.x] = Type(1);
      vAngles[axes.y] = Type(0);
      vAngles[axes.z] = Type(0);
    }
  };

  // Negation
  __forceinline QUAT operator-(void) const {
    return QUAT(-m_w, -m_x, -m_y, -m_z);
  };

  // Conjugation
  __forceinline QUAT operator~(void) const {
    return QUAT(m_w, -m_x, -m_y, -m_z);
  };

  // Inversion
  __forceinline QUAT Invert(void) const {
    return ~(*this) / Norm();
  };

  // Quaternion norm
  __forceinline Type Norm(void) const {
    return ToVector().Length();
  };

  // Assignment from another quaternion
  __forceinline QUAT &operator=(const QUAT &qOther) {
    if (&qOther == this) return *this;

    m_w = qOther.m_w;
    m_x = qOther.m_x;
    m_y = qOther.m_y;
    m_z = qOther.m_z;
    return *this;
  };

  // Assignment from a vector
  __forceinline QUAT &operator=(const Vector &vOther) {
    m_w = vOther[0];
    m_x = vOther[1];
    m_y = vOther[2];
    m_z = vOther[3];
    return *this;
  };

// Arithmetic operations
public:

  // Add another quaternion
  __forceinline QUAT operator+(const QUAT &qOther) const {
    return ToVector() + qOther.ToVector();
  };

  // Add another quaternion
  __forceinline QUAT &operator+=(const QUAT &qOther) {
    (*this) = ToVector() + qOther.ToVector();
    return *this;
  };

  // Subtract another quaternion
  __forceinline QUAT operator-(const QUAT &qOther) const {
    return ToVector() - qOther.ToVector();
  };

  // Subtract another quaternion
  __forceinline QUAT &operator-=(const QUAT &qOther) {
    (*this) = ToVector() - qOther.ToVector();
    return *this;
  };

  // Multiply by another quaternion
  __forceinline QUAT operator*(const QUAT &qOther) const {
    return ToVector() * qOther.ToVector();
  };

  // Multiply by another quaternion
  __forceinline QUAT &operator*=(const QUAT &qOther) {
    (*this) = ToVector() * qOther.ToVector();
    return *this;
  };

  // Multiply by a factor
  __forceinline QUAT operator*(const Type factor) const {
    return ToVector() * factor;
  };

  // Multiply by a factor
  __forceinline QUAT &operator*=(const Type factor) {
    (*this) = ToVector() * factor;
    return *this;
  };

  // Divide by a factor
  __forceinline QUAT operator/(const Type factor) const {
    return ToVector() / factor;
  };

  // Divide by a factor
  __forceinline QUAT &operator/=(const Type factor) {
    (*this) = ToVector() / factor;
    return *this;
  };

  // Dot product between two quaternions
  __forceinline Type operator%(const QUAT &qOther) const {
    return ToVector() % qOther.ToVector();
  };
};

#undef QUAT

// Standard quaternions
typedef TQuaternion<f32> quatf; // Single precision quaternion
typedef TQuaternion<f64> quatd; // Double precision quaternion

NAMESPACE_DREAMY_CLOSE

#endif // (Dreamy Utilities Include Guard)
