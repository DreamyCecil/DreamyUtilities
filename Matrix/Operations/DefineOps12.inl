//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#if !defined(MATRIX_TYPE)
  #error Please define matrix type as 'MATRIX_TYPE' before defining 4x3 matrix operations.
#endif

// Matrix type
#define ROTMAT TMatrix<MATRIX_TYPE, 3, 4>

// Transpose the matrix using another matrix
template<> inline
ROTMAT &ROTMAT::operator!=(const ROTMAT &mOther) {
  ROTMAT &m = *this;

  m(0, 0) = mOther(0, 0);
  m(1, 1) = mOther(1, 1);
  m(2, 2) = mOther(2, 2);
  m(0, 3) = mOther(0, 3);
  m(1, 3) = mOther(1, 3);
  m(2, 3) = mOther(2, 3);

  m(0, 1) = mOther(1, 0);
  m(0, 2) = mOther(2, 0);
  m(1, 0) = mOther(0, 1);
  m(2, 0) = mOther(0, 2);
  m(1, 2) = mOther(2, 1);
  m(2, 1) = mOther(1, 2);

  m(0, 3) = -m(0, 0) * mOther(0, 3) - m(0, 1) * mOther(1, 3) - m(0, 2) * mOther(2, 3);
  m(1, 3) = -m(1, 0) * mOther(0, 3) - m(1, 1) * mOther(1, 3) - m(1, 2) * mOther(2, 3);
  m(2, 3) = -m(2, 0) * mOther(0, 3) - m(2, 1) * mOther(1, 3) - m(2, 2) * mOther(2, 3);

  return *this;
};

// Multiply by another matrix
template<> inline
ROTMAT ROTMAT::operator*(const ROTMAT &mOther) const {
  const ROTMAT &m = *this;
  ROTMAT mResult;

  mResult(0, 0) = m(0, 0) * mOther(0, 0) + m(0, 1) * mOther(1, 0) + m(0, 2) * mOther(2, 0);
  mResult(0, 1) = m(0, 0) * mOther(0, 1) + m(0, 1) * mOther(1, 1) + m(0, 2) * mOther(2, 1);
  mResult(0, 2) = m(0, 0) * mOther(0, 2) + m(0, 1) * mOther(1, 2) + m(0, 2) * mOther(2, 2);
  mResult(0, 3) = m(0, 0) * mOther(0, 3) + m(0, 1) * mOther(1, 3) + m(0, 2) * mOther(2, 3) + m(0, 3);

  mResult(1, 0) = m(1, 0) * mOther(0, 0) + m(1, 1) * mOther(1, 0) + m(1, 2) * mOther(2, 0);
  mResult(1, 1) = m(1, 0) * mOther(0, 1) + m(1, 1) * mOther(1, 1) + m(1, 2) * mOther(2, 1);
  mResult(1, 2) = m(1, 0) * mOther(0, 2) + m(1, 1) * mOther(1, 2) + m(1, 2) * mOther(2, 2);
  mResult(1, 3) = m(1, 0) * mOther(0, 3) + m(1, 1) * mOther(1, 3) + m(1, 2) * mOther(2, 3) + m(1, 3);

  mResult(2, 0) = m(2, 0) * mOther(0, 0) + m(2, 1) * mOther(1, 0) + m(2, 2) * mOther(2, 0);
  mResult(2, 1) = m(2, 0) * mOther(0, 1) + m(2, 1) * mOther(1, 1) + m(2, 2) * mOther(2, 1);
  mResult(2, 2) = m(2, 0) * mOther(0, 2) + m(2, 1) * mOther(1, 2) + m(2, 2) * mOther(2, 2);
  mResult(2, 3) = m(2, 0) * mOther(0, 3) + m(2, 1) * mOther(1, 3) + m(2, 2) * mOther(2, 3) + m(2, 3);

  return mResult;
};

#undef ROTMAT
