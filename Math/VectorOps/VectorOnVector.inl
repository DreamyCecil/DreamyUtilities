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

// Define operations for vectors with vectors
#define DO_DIMENSION_OPERATION(VecIndex, ThisVec, Operation, Other) ThisVec[VecIndex] Operation Other[VecIndex];
#include "OpsDefine.inl"

// Add another vector
template<>
inline NUMVEC NUMVEC::operator+(const NUMVEC &vOther) const {
  NUMVEC v(*this);
  DO_VECTOR_OPERATION(v, +=, vOther);
  return v;
};

template<>
inline NUMVEC &NUMVEC::operator+=(const NUMVEC &vOther) {
  DO_VECTOR_OPERATION(m_aValues, +=, vOther);
  return *this;
};

// Subtract another vector
template<>
inline NUMVEC NUMVEC::operator-(const NUMVEC &vOther) const {
  NUMVEC v(*this);
  DO_VECTOR_OPERATION(v, -=, vOther);
  return v;
};

template<>
inline NUMVEC &NUMVEC::operator-=(const NUMVEC &vOther) {
  DO_VECTOR_OPERATION(m_aValues, -=, vOther);
  return *this;
};

// Multiply by another vector
template<>
inline NUMVEC NUMVEC::operator*(const NUMVEC &vOther) const {
  NUMVEC v(*this);
  DO_VECTOR_OPERATION(v, *=, vOther);
  return v;
};

template<>
inline NUMVEC &NUMVEC::operator*=(const NUMVEC &vOther) {
  DO_VECTOR_OPERATION(m_aValues, *=, vOther);
  return *this;
};

// Divide by another vector
template<>
inline NUMVEC NUMVEC::operator/(const NUMVEC &vOther) const {
  NUMVEC v(*this);
  DO_VECTOR_OPERATION(v, /=, vOther);
  return v;
};

template<>
inline NUMVEC &NUMVEC::operator/=(const NUMVEC &vOther) {
  DO_VECTOR_OPERATION(m_aValues, /=, vOther);
  return *this;
};

// Negate the vector
template<>
inline NUMVEC NUMVEC::operator-(void) const {
  NUMVEC v(*this);
  DO_VECTOR_OPERATION(v, =, -v);
  return v;
};

#include "OpsUndef.inl"
