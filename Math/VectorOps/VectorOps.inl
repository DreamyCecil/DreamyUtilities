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

#if !defined(NUMVEC_OPS_FOR)
  #error Please define amount of dimensions for 'NUMVEC_OPS_FOR' before operation definitions.

#elif !defined(VECTOR_TYPE)
  #error Please define the type for 'VECTOR_TYPE' before operation definitions.
#endif

#include "VectorOnVector.inl"
#include "VectorOnFactor.inl"

// Define operations for adding up vector values
#define DO_DIMENSION_OPERATION(VecIndex, ThisVec, Operation, Other) +(ThisVec[VecIndex] Operation Other[VecIndex])
#include "OpsDefine.inl"

// Dot product between two vectors
template<>
inline VECTOR_TYPE NUMVEC::operator%(const NUMVEC &vOther) const {
  // Compiles as "total = + (this[0] * other[0]) + (this[1] * other[1]) ..."
  VECTOR_TYPE total =  DO_VECTOR_OPERATION(m_aValues, *, vOther);
  return total;
};

// Get length of the vector
template<>
inline VECTOR_TYPE NUMVEC::Length(void) const {
  VECTOR_TYPE total = (*this) % (*this);
  return (VECTOR_TYPE)sqrt((f64)total);
};

#include "OpsUndef.inl"

#undef NUMVEC_OPS_FOR
