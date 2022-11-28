//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_QUATERNION_H
#define _DREAMYUTILITIES_INCL_QUATERNION_H

#include "../Base/Base.hpp"

#include "../Vector/NumericVector.hpp"
#include "../Matrix/ScalarMatrices.hpp"

namespace dreamy
{
  #define QUAT TQuaternion<Type>

  // Quaternion of a certain type
  template<typename Type> class TQuaternion {
    public:
      typedef Type T; // Template type
      typedef TNumVec<Type, 4> QuatVector;

    public:
      union {
        Type _values[4];
        struct { Type _w, _x, _y, _z; };
      };

    public:
      // Default constructor
      __forceinline TQuaternion(void)
      {
      };

      // Value constructor
      __forceinline TQuaternion(Type wSet, Type xSet, Type ySet, Type zSet) :
        _w(wSet), _x(xSet), _y(ySet), _z(zSet)
      {
      };

      // Vector constructor
      __forceinline TQuaternion(const QuatVector &v) :
        _w(v[0]), _x(v[1]), _y(v[2]), _z(v[3])
      {
      };

      // Convert into a vector
      __forceinline QuatVector ToVector(void) const {
        return QuatVector(_w, _x, _y, _z);
      };

      // Convert from euler angles (H, P, B) in radians
      void FromEuler(const TNumVec<Type, 3> &vAngles, const VecAxes axes = _DREAMY_DEFAULT_AXES) {
        Type cosH = cos(vAngles[0] * Type(0.5));
        Type sinH = sin(vAngles[0] * Type(0.5));
        Type cosP = cos(vAngles[1] * Type(0.5));
        Type sinP = sin(vAngles[1] * Type(0.5));
        Type cosB = cos(vAngles[2] * Type(0.5));
        Type sinB = sin(vAngles[2] * Type(0.5));

        _w = cosB * cosP * cosH + sinB * sinP * sinH;

        // From Wikipedia, offsetted for 3D space with upward Z axis
        //_x = sinB * cosP * cosH - cosB * sinP * sinH;
        //_y = cosB * sinP * cosH + sinB * cosP * sinH;
        //_z = cosB * cosP * sinH - sinB * sinP * cosH;

        // ZXY/201 order by default for 3D space with upward Y axis
        (&_x)[axes._x] = sinB * cosP * cosH - cosB * sinP * sinH;
        (&_x)[axes._y] = cosB * sinP * cosH + sinB * cosP * sinH;
        (&_x)[axes._z] = cosB * cosP * sinH - sinB * sinP * cosH;
      };

      // Convert to euler angles (H, P, B) in radians
      void ToEuler(TNumVec<Type, 3> &vAngles, const VecAxes axes = _DREAMY_DEFAULT_AXES) {
        // From Wikipedia, offsetted for 3D space with upward Z axis
        //Type axisValue[3] = { _x, _y, _z };

        // ZXY/201 order by default for 3D space with upward Y axis
        Type axisValue[3] = { (&_x)[axes._x], (&_x)[axes._y], (&_x)[axes._z] };

        // Heading
        Type sinH_cosP = 2 * (_w * axisValue[2] + axisValue[0] * axisValue[1]);
        Type cosH_cosP = 1 - 2 * (axisValue[1] * axisValue[1] + axisValue[2] * axisValue[2]);
        vAngles[0] = atan2(sinH_cosP, cosH_cosP);

        // Pitch
        Type sinP = 2 * (_w * axisValue[1] - axisValue[2] * axisValue[0]);

        if (dreamy::math::Abs(sinP) >= 1) {
          vAngles[1] = dreamy::math::CopySign(Type(dreamy::math::PI / 2), sinP); // Use 90 degrees if out of range
        } else {
          vAngles[1] = asin(sinP);
        }

        // Banking
        Type sinB_cosP = 2 * (_w * axisValue[0] + axisValue[1] * axisValue[2]);
        Type cosB_cosP = 1 - 2 * (axisValue[0] * axisValue[0] + axisValue[1] * axisValue[1]);
        vAngles[2] = atan2(sinB_cosP, cosB_cosP);
      };

      // Convert to a rotation matrix
      void ToMatrix(TMatrix<Type, 3, 3> &matrix) const {
        Type wx = 2 * _w * _x;
        Type wy = 2 * _w * _y;
        Type wz = 2 * _w * _z;

        Type xx = 2 * _x * _x;
        Type xy = 2 * _x * _y;
        Type xz = 2 * _x * _z;

        Type yy = 2 * _y * _y;
        Type yz = 2 * _y * _z;
        Type zz = 2 * _z * _z;

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
          root = sqrt(trace + Type(1.0)); // 2w
          _w = Type(0.5) * root;
          root = Type(0.5) / root; // 1/(4w)

          // ZXY/201 order by default for 3D space with upward Y axis
          _x = (matrix(2, 1) - matrix(1, 2)) * root;
          _y = (matrix(0, 2) - matrix(2, 0)) * root;
          _z = (matrix(1, 0) - matrix(0, 1)) * root;

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

          root = sqrt(matrix(i, i) - matrix(j, j) - matrix(k, k) + Type(1.0));

          Type *quat[3] = { &_x, &_y, &_z };
          *quat[i] = Type(0.5) * root;

          root = Type(0.5) / root;
          _w = (matrix(k, j) - matrix(j, k)) * root;

          *quat[j] = (matrix(j, i) + matrix(i, j)) * root;
          *quat[k] = (matrix(k, i) + matrix(i, k)) * root;
        }
      };

      // Convert from axis angle (in radians)
      void FromAxisAngle(const TNumVec<Type, 3> &vAngles, const Type angle, const VecAxes axes = VecAxes()) {
        Type sinAngle = sin(angle / 2);

        _w = cos(angle / 2);
        _x = vAngles[axes._x] * sinAngle;
        _y = vAngles[axes._y] * sinAngle;
        _z = vAngles[axes._z] * sinAngle;
      };

      // Convert to axis angle (in radians)
      void ToAxisAngle(TNumVec<Type, 3> &vAngles, Type &angle, const VecAxes axes = VecAxes()) {
        Type sinAngle = sqrt(Type(1) - _w * _w);
        angle = 2 * acos(_w);

        // Angle isn't zero
        if (dreamy::math::Abs(sinAngle) >= 0.001) {
          vAngles[axes._x] = _x / sinAngle;
          vAngles[axes._y] = _y / sinAngle;
          vAngles[axes._z] = _z / sinAngle;

        // Angle is zero
        } else {
          vAngles[axes._x] = Type(1);
          vAngles[axes._y] = Type(0);
          vAngles[axes._z] = Type(0);
        }
      };

      // Negation
      __forceinline QUAT operator-(void) const {
        return QUAT(-_w, -_x, -_y, -_z);
      };

      // Conjugation
      __forceinline QUAT operator~(void) const {
        return QUAT(_w, -_x, -_y, -_z);
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

        _w = qOther._w;
        _x = qOther._x;
        _y = qOther._y;
        _z = qOther._z;
        return *this;
      };

      // Assignment from a vector
      __forceinline QUAT &operator=(const QuatVector &vOther) {
        _w = vOther[0];
        _x = vOther[1];
        _y = vOther[2];
        _z = vOther[3];
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
  typedef TQuaternion<f32> QuatF; // Single precision quaternion
  typedef TQuaternion<f64> QuatD; // Double precision quaternion

};

#endif // (Dreamy Utilities Include Guard)
