//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_SCALARMATRICES_H
#define _DREAMYUTILITIES_INCL_SCALARMATRICES_H

#include "../Base/Base.hpp"

#include "RotationMatrix.hpp"

namespace dreamy
{
  // Single precision matrices
  typedef TMatrix<f32, 2, 2> Mat2F; // 2x2 single precision matrix
  typedef TMatrix<f32, 3, 3> Mat3F; // 3x3 single precision matrix

  // Define operators
  #define MATRIX_TYPE f32
  #include "Operations/DefineOps2D.inl"
  #include "Operations/DefineOps3D.inl"
  #undef MATRIX_TYPE

  // Double precision matrices
  typedef TMatrix<f64, 2, 2> Mat2D; // 2x2 double precision matrix
  typedef TMatrix<f64, 3, 3> Mat3D; // 3x3 double precision matrix

  // Define operators
  #define MATRIX_TYPE f64
  #include "Operations/DefineOps2D.inl"
  #include "Operations/DefineOps3D.inl"
  #undef MATRIX_TYPE

};

#endif // (Dreamy Utilities Include Guard)
