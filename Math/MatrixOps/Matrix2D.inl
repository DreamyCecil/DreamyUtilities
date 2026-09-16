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
template<>
inline ROTMAT ROTMAT::operator*(const ROTMAT &mOther) const {
  ROTMAT mResult;

  mResult(0, 0) = (*this)(0, 0) * mOther(0, 0) + (*this)(0, 1) * mOther(1, 0);
  mResult(0, 1) = (*this)(0, 0) * mOther(0, 1) + (*this)(0, 1) * mOther(1, 1);
  mResult(1, 0) = (*this)(1, 0) * mOther(0, 0) + (*this)(1, 1) * mOther(1, 0);
  mResult(1, 1) = (*this)(1, 0) * mOther(0, 1) + (*this)(1, 1) * mOther(1, 1);

  return mResult;
};

#undef ROTMAT
