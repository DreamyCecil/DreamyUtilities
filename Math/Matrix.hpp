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

#ifndef _DREAMYUTILITIES_INCL_ROTATIONMATRIX_H
#define _DREAMYUTILITIES_INCL_ROTATIONMATRIX_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

#include "Math.hpp"
#include "Vector.hpp"

NAMESPACE_DREAMY_OPEN

// Matrix type
#define ROTMAT TMatrix<Type, iRows, iCols>

// Set all dimensions in the matrix to a certain value
#define ROTMAT_SET_ALL(MatrixValue) { \
  s32 i = iRows * iCols; \
  while (--i >= 0) _matrix[i % iRows][i / iRows] = Type(MatrixValue); \
}

// Copy all dimensions from another matrix to this one
#define ROTMAT_COPY_ALL(OtherMatrix) { \
  s32 i = iRows * iCols; \
  while (--i >= 0) _matrix[i % iRows][i / iRows] = OtherMatrix[i % iRows][i / iRows]; \
}

template<typename Type, const u32 iRows, const u32 iCols>
class TMatrix {

public:
  typedef Type T; // Template type
  typedef TVector<Type, iRows> MatrixRow;      // Row of values
  typedef TVector<Type, iCols> MatrixColumn;   // Column of values
  typedef TVector<MatrixColumn, iRows> Matrix; // Row of columns

protected:
  Matrix _matrix;

public:
  // Default constructor
  __forceinline TMatrix(const Type val = 0) {
    ROTMAT_SET_ALL(val);
  };

  // Array constructor
  __forceinline TMatrix(const Type aValues[iRows][iCols]) {
    ROTMAT_COPY_ALL(aValues);
  };

  // Copy constructor
  __forceinline TMatrix(const ROTMAT &mOther) {
    ROTMAT_COPY_ALL(mOther);
  };

  // Clear the matrix
  __forceinline void Clear(void) {
    ROTMAT_SET_ALL(0);
  };

  // Transpose the matrix (top-left to bottom-right mirroring)
  __forceinline ROTMAT TransposeTL2BR(void) const {
    return ROTMAT().TransposeTL2BR(*this);
  };

  // Transpose the matrix using another matrix (top-left to bottom-right mirroring)
  __forceinline ROTMAT &TransposeTL2BR(const ROTMAT &mOther) {
    for (u32 iRow = 0; iRow < iRows; ++iRow)
    {
      for (u32 iCol = 0; iCol < iCols; ++iCol)
      {
        (*this)(iCol, iRow) = mOther(iRow, iCol);
      }
    }

    return *this;
  };

  // Transpose the matrix using another matrix (top-right to bottom-left mirroring)
  __forceinline ROTMAT &TransposeTR2BL(const ROTMAT &mOther) {
    for (u32 iRow = 0; iRow < iRows; ++iRow)
    {
      for (u32 iCol = 0; iCol < iCols; ++iCol)
      {
        (*this)(iCol, iRow) = mOther(iRows - iRow - 1, iCols - iCol - 1);
      }
    }

    return *this;
  };

  // Access matrix value
  __forceinline Type &operator()(s32 iRow, s32 iCol) {
    return _matrix[iRow][iCol];
  };

  // Access matrix value (read-only)
  __forceinline const Type &operator()(s32 iRow, s32 iCol) const {
    return _matrix[iRow][iCol];
  };

  // Access matrix row
  __forceinline MatrixColumn &operator[](s32 iRow) {
    return _matrix[iRow];
  };

  // Access matrix row (read-only)
  __forceinline const MatrixColumn &operator[](s32 iRow) const {
    return _matrix[iRow];
  };

  // Get matrix row
  __forceinline MatrixColumn Row(s32 iRow) const {
    return _matrix[iRow];
  };

  // Get matrix column
  __forceinline MatrixRow Column(s32 iCol) const {
    MatrixRow vRow;

    s32 i = iRows;
    while (--i >= 0) {
      vRow[i] = _matrix[i][iCol];
    }

    return vRow;
  };

// Comparison
public:

  // Check if matrices are the same
  __forceinline bool operator==(const ROTMAT &mOther) const {
    s32 i = iRows;
    while (--i >= 0) {
      // At least one row doesn't match
      if (_matrix[i] != mOther[i]) {
        return false;
      }
    }
    return true;
  };

  // Check if matrices are different
  __forceinline bool operator!=(const ROTMAT &mOther) const {
    return !operator==(mOther);
  };

// Arithmetic operations
public:

  // Multiply by another matrix
  ROTMAT operator*(const ROTMAT &mOther) const;

  // Multiply by another matrix
  ROTMAT &operator*=(const ROTMAT &mOther);

// Operations between vectors and matrices
public:

  // Vector with the same amount of dimensions as matrix rows
  #define NUMVEC TVector<Type, iRows>

  // Apply rotation matrix to the vector
  friend NUMVEC operator*(const NUMVEC &v, const ROTMAT &matrix) {
    NUMVEC vResult;

    for (u32 iRow = 0; iRow < iRows; ++iRow) {
      for (u32 iCol = 0; iCol < iCols; ++iCol) {
        vResult[iRow] += matrix(iRow, iCol) * v[iCol];
      }
    }

    return vResult;
  };

  // Apply rotation matrix to the vector
  friend NUMVEC &operator*=(NUMVEC &v, const ROTMAT &matrix) {
    v = v * matrix;
    return v;
  };

  #undef NUMVEC
};

// Multiply by another matrix
template<typename Type, const u32 iRows, const u32 iCols>
inline ROTMAT ROTMAT::operator*(const ROTMAT &mOther) const {
  ROTMAT mResult;

  for (u32 iRow = 0; iRow < iRows; ++iRow) {
    for (u32 iCol = 0; iCol < iCols; ++iCol) {
      mResult(iRow, iCol) = (Type)0;

      for (u32 iSpecial = 0; iSpecial < iRows; ++iSpecial) {
        mResult(iRow, iCol) += (*this)(iRow, iSpecial) * mOther(iSpecial, iCol);
      }
    }
  }

  return mResult;
};

// Multiply by another matrix
template<typename Type, const u32 iRows, const u32 iCols>
inline ROTMAT &ROTMAT::operator*=(const ROTMAT &mOther) {
  (*this) = (*this) * mOther;
  return *this;
};

#undef ROTMAT

NAMESPACE_DREAMY_CLOSE

#endif // (Dreamy Utilities Include Guard)