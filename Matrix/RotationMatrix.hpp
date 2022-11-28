//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_ROTATIONMATRIX_H
#define _DREAMYUTILITIES_INCL_ROTATIONMATRIX_H

#include "../Base/Base.hpp"

#include "../Vector/NumericVector.hpp"
#include "../Math/Trigonometry.hpp"

namespace dreamy
{
  // Matrix template and type
  #define ROTMAT_TEMP template<typename Type, const u32 iRows, const u32 iCols>
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

  ROTMAT_TEMP class TMatrix {
    public:
      typedef Type T; // Template type
      typedef TNumVec<Type, iRows> MatrixRow;      // Row of values
      typedef TNumVec<Type, iCols> MatrixColumn;   // Column of values
      typedef TNumVec<MatrixColumn, iRows> Matrix; // Row of columns

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
      __forceinline ROTMAT operator!(void) const {
        return ROTMAT() != *this;
      };

      // Transpose the matrix using another matrix (top-left to bottom-right mirroring)
      __forceinline ROTMAT &operator!=(const ROTMAT &mOther) {
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
      __forceinline ROTMAT &operator%=(const ROTMAT &mOther) {
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
      __forceinline Type &operator()(const s32 iRow, const s32 iCol) {
        return _matrix[iRow][iCol];
      };

      // Access matrix value (read-only)
      __forceinline const Type &operator()(const s32 iRow, const s32 iCol) const {
        return _matrix[iRow][iCol];
      };

      // Access matrix row
      __forceinline MatrixColumn &operator[](const s32 iRow) {
        return _matrix[iRow];
      };

      // Access matrix row (read-only)
      __forceinline const MatrixColumn &operator[](const s32 iRow) const {
        return _matrix[iRow];
      };

      // Get matrix row
      __forceinline MatrixColumn Row(const s32 iRow) const {
        return _matrix[iRow];
      };

      // Get matrix column
      __forceinline MatrixRow Column(const s32 iCol) const {
        MatrixRow vRow;

        s32 i = iRows;
        while (--i >= 0) {
          vRow[i] = _matrix[i][iCol];
        }

        return vRow;
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
      #define NUMVEC TNumVec<Type, iRows>

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
  ROTMAT_TEMP inline
  ROTMAT ROTMAT::operator*(const ROTMAT &mOther) const {
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
  ROTMAT_TEMP inline
  ROTMAT &ROTMAT::operator*=(const ROTMAT &mOther) {
    (*this) = (*this) * mOther;
    return *this;
  };

  // Compare two matrices
  ROTMAT_TEMP
  bool CompareMatrices(const ROTMAT &m1, const ROTMAT &m2) {
    s32 i = iRows;

    while (--i >= 0) {
      // At least one row doesn't match
      if (m1[i] != m2[i]) {
        return false;
      }
    }

    return true;
  };

  #undef ROTMAT
  #undef ROTMAT_TEMP

};

#endif // (Dreamy Utilities Include Guard)