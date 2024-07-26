//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#if !defined(MATRIX_TYPE)
  #error Please define matrix type as 'MATRIX_TYPE' before defining 2D matrix operations.
#endif

// Matrix type
#define ROTMAT TMatrix<MATRIX_TYPE, 2, 2>

// Make 2D rotation matrix out of an angle in radians
inline void Mat2DFromAngle(ROTMAT &m, const MATRIX_TYPE angle) {
  MATRIX_TYPE sinAngle((MATRIX_TYPE)sin(angle));
  MATRIX_TYPE cosAngle((MATRIX_TYPE)cos(angle));

  m(0, 0) = cosAngle;
  m(0, 1) = -sinAngle;
  m(1, 0) = sinAngle;
  m(1, 1) = cosAngle;
};

// Multiply by another matrix
template<> inline
ROTMAT ROTMAT::operator*(const ROTMAT &mOther) const {
  ROTMAT mResult;

  mResult(0, 0) = (*this)(0, 0) * mOther(0, 0) + (*this)(0, 1) * mOther(1, 0);
  mResult(0, 1) = (*this)(0, 0) * mOther(0, 1) + (*this)(0, 1) * mOther(1, 1);
  mResult(1, 0) = (*this)(1, 0) * mOther(0, 0) + (*this)(1, 1) * mOther(1, 0);
  mResult(1, 1) = (*this)(1, 0) * mOther(0, 1) + (*this)(1, 1) * mOther(1, 1);

  return mResult;
};

#undef ROTMAT
