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

#if !defined(MATRIX_TYPE)
  #error Please define matrix type as 'MATRIX_TYPE' before defining 3D matrix operations.
#endif

// Matrix type
#define ROTMAT TMatrix<MATRIX_TYPE, 3, 3>

// Transpose the matrix using another matrix (top-left to bottom-right mirroring)
template<>
inline ROTMAT &ROTMAT::TransposeTL2BR(const ROTMAT &mOther) {
  ROTMAT &m = *this;

  // Mirror by | \.. |
  // diagonal  | .\. |
  // TL -> BR  | ..\ |
  m(0, 0) = mOther(0, 0);
  m(0, 1) = mOther(1, 0);
  m(0, 2) = mOther(2, 0);

  m(1, 0) = mOther(0, 1);
  m(1, 1) = mOther(1, 1);
  m(1, 2) = mOther(2, 1);

  m(2, 0) = mOther(0, 2);
  m(2, 1) = mOther(1, 2);
  m(2, 2) = mOther(2, 2);

  return *this;
};

// Transpose the matrix using another matrix (top-right to bottom-left mirroring)
template<>
inline ROTMAT &ROTMAT::TransposeTR2BL(const ROTMAT &mOther) {
  ROTMAT &m = *this;

  // Mirror by | ../ |
  // diagonal  | ./. |
  // TR -> BL  | /.. |
  m(0, 0) = mOther(2, 2);
  m(0, 1) = mOther(1, 2);
  m(0, 2) = mOther(0, 2);

  m(1, 0) = mOther(2, 1);
  m(1, 1) = mOther(1, 1);
  m(1, 2) = mOther(0, 1);

  m(2, 0) = mOther(2, 0);
  m(2, 1) = mOther(1, 0);
  m(2, 2) = mOther(0, 0);

  return *this;
};

// Multiply by another matrix
template<>
inline ROTMAT ROTMAT::operator*(const ROTMAT &mOther) const {
  ROTMAT mResult;

  mResult(0, 0) = (*this)(0, 0) * mOther(0, 0) + (*this)(0, 1) * mOther(1, 0) + (*this)(0, 2) * mOther(2, 0);
  mResult(0, 1) = (*this)(0, 0) * mOther(0, 1) + (*this)(0, 1) * mOther(1, 1) + (*this)(0, 2) * mOther(2, 1);
  mResult(0, 2) = (*this)(0, 0) * mOther(0, 2) + (*this)(0, 1) * mOther(1, 2) + (*this)(0, 2) * mOther(2, 2);
  mResult(1, 0) = (*this)(1, 0) * mOther(0, 0) + (*this)(1, 1) * mOther(1, 0) + (*this)(1, 2) * mOther(2, 0);
  mResult(1, 1) = (*this)(1, 0) * mOther(0, 1) + (*this)(1, 1) * mOther(1, 1) + (*this)(1, 2) * mOther(2, 1);
  mResult(1, 2) = (*this)(1, 0) * mOther(0, 2) + (*this)(1, 1) * mOther(1, 2) + (*this)(1, 2) * mOther(2, 2);
  mResult(2, 0) = (*this)(2, 0) * mOther(0, 0) + (*this)(2, 1) * mOther(1, 0) + (*this)(2, 2) * mOther(2, 0);
  mResult(2, 1) = (*this)(2, 0) * mOther(0, 1) + (*this)(2, 1) * mOther(1, 1) + (*this)(2, 2) * mOther(2, 1);
  mResult(2, 2) = (*this)(2, 0) * mOther(0, 2) + (*this)(2, 1) * mOther(1, 2) + (*this)(2, 2) * mOther(2, 2);

  return mResult;
};

// Make 3D rotation matrix out of euler angles (H, P, B) in radians
inline void Mat3DFromAngles(ROTMAT &m, const TVector<MATRIX_TYPE, 3> &vAngles, const VecAxes axes = VecAxes()) {
  // Heading
  MATRIX_TYPE sinH = (MATRIX_TYPE)sin(vAngles[axes.x]);
  MATRIX_TYPE cosH = (MATRIX_TYPE)cos(vAngles[axes.x]);
  // Pitch
  MATRIX_TYPE sinP = (MATRIX_TYPE)sin(vAngles[axes.y]);
  MATRIX_TYPE cosP = (MATRIX_TYPE)cos(vAngles[axes.y]);
  // Banking
  MATRIX_TYPE sinB = (MATRIX_TYPE)sin(vAngles[axes.z]);
  MATRIX_TYPE cosB = (MATRIX_TYPE)cos(vAngles[axes.z]);

  m(0, 0) = cosH * cosB + sinP * sinH * sinB;
  m(0, 1) = sinP * sinH * cosB - cosH * sinB;
  m(0, 2) = cosP * sinH;
  m(1, 0) = cosP * sinB;
  m(1, 1) = cosP * cosB;
  m(1, 2) = -sinP;
  m(2, 0) = sinP * cosH * sinB - sinH * cosB;
  m(2, 1) = sinP * cosH * cosB + sinH * sinB;
  m(2, 2) = cosP * cosH;
};

// Make inverted 3D rotation matrix out of euler angles (H, P, B) in radians
inline void Mat3DFromAnglesInverse(ROTMAT &m, const TVector<MATRIX_TYPE, 3> &vAngles, const VecAxes axes = VecAxes()) {
  // Heading
  MATRIX_TYPE sinH = (MATRIX_TYPE)sin(vAngles[axes.x]);
  MATRIX_TYPE cosH = (MATRIX_TYPE)cos(vAngles[axes.x]);
  // Pitch
  MATRIX_TYPE sinP = (MATRIX_TYPE)sin(vAngles[axes.y]);
  MATRIX_TYPE cosP = (MATRIX_TYPE)cos(vAngles[axes.y]);
  // Banking
  MATRIX_TYPE sinB = (MATRIX_TYPE)sin(vAngles[axes.z]);
  MATRIX_TYPE cosB = (MATRIX_TYPE)cos(vAngles[axes.z]);

  m(0, 0) = cosH * cosB + sinP * sinH * sinB;
  m(1, 0) = sinP * sinH * cosB - cosH * sinB;
  m(2, 0) = cosP * sinH;
  m(0, 1) = cosP * sinB;
  m(1, 1) = cosP * cosB;
  m(2, 1) = -sinP;
  m(0, 2) = sinP * cosH * sinB - sinH * cosB;
  m(1, 2) = sinP * cosH * cosB + sinH * sinB;
  m(2, 2) = cosP * cosH;
};

// Convert rotation matrix into euler angles (H, P, B) in radians
inline void Mat3DToAngles(const ROTMAT &m, TVector<MATRIX_TYPE, 3> &vAngles, const VecAxes axes = VecAxes()) {
  MATRIX_TYPE &h = vAngles[axes.x];
  MATRIX_TYPE &p = vAngles[axes.y];
  MATRIX_TYPE &b = vAngles[axes.z];

  // Calculate pitch
  MATRIX_TYPE f23 = m(1, 2);
  p = (MATRIX_TYPE)asin(-f23);

  // If pitch makes banking be the same as heading
  if (dreamy::math::SqrtSign(1.0 - f23 * f23) < 0.001) {
    // Set banking to zero
    b = 0;

    // and calculate heading for that
    h = (MATRIX_TYPE)atan2(m(0, 1) / -m(1, 2), m(0, 0));

  } else {
    // Calculate banking and heading normally
    b = (MATRIX_TYPE)atan2(m(1, 0), m(1, 1));
    h = (MATRIX_TYPE)atan2(m(0, 2), m(2, 2));
  }
};

#undef ROTMAT
