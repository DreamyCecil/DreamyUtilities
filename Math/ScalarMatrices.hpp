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

#ifndef _DREAMYUTILITIES_INCL_SCALARMATRICES_H
#define _DREAMYUTILITIES_INCL_SCALARMATRICES_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

#include "Matrix.hpp"

NAMESPACE_DREAMY_OPEN

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

NAMESPACE_DREAMY_CLOSE

#endif // (Dreamy Utilities Include Guard)
