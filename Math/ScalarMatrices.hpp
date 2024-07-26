//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_SCALARMATRICES_H
#define _DREAMYUTILITIES_INCL_SCALARMATRICES_H

#include "../DreamyUtilitiesBase.hpp"

#include "Matrix.hpp"

namespace dreamy {

// Single precision matrices
typedef TMatrix<f32, 2, 2> mat2f; // 2x2 single precision matrix
typedef TMatrix<f32, 3, 3> mat3f; // 3x3 single precision matrix

// Define operators
#define MATRIX_TYPE f32
#include "MatrixOps/Matrix2D.inl"
#include "MatrixOps/Matrix3D.inl"
#undef MATRIX_TYPE

// Double precision matrices
typedef TMatrix<f64, 2, 2> mat2d; // 2x2 double precision matrix
typedef TMatrix<f64, 3, 3> mat3d; // 3x3 double precision matrix

// Define operators
#define MATRIX_TYPE f64
#include "MatrixOps/Matrix2D.inl"
#include "MatrixOps/Matrix3D.inl"
#undef MATRIX_TYPE

};

#endif // (Dreamy Utilities Include Guard)
