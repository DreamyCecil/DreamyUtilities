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

// Define operations for vectors with factors
#define DO_DIMENSION_OPERATION(VecIndex, ThisVec, Operation, Other) ThisVec[VecIndex] Operation Other;
#include "OpsDefine.inl"

// Multiply vector by a factor
template<>
inline NUMVEC NUMVEC::operator*(const VECTOR_TYPE factor) const {
  NUMVEC v(*this);
  DO_VECTOR_OPERATION(v, *=, factor);
  return v;
};

template<>
inline NUMVEC &NUMVEC::operator*=(const VECTOR_TYPE factor) {
  DO_VECTOR_OPERATION(m_aValues, *=, factor);
  return *this;
};

// Divide vector by a factor
template<>
inline NUMVEC NUMVEC::operator/(const VECTOR_TYPE factor) const {
  NUMVEC v(*this);
  DO_VECTOR_OPERATION(v, /=, factor);
  return v;
};

template<>
inline NUMVEC &NUMVEC::operator/=(const VECTOR_TYPE factor) {
  DO_VECTOR_OPERATION(m_aValues, /=, factor);
  return *this;
};

#include "OpsUndef.inl"
