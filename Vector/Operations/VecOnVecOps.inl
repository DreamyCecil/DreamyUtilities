//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

// Define operations for vectors with vectors
#define DO_DIMENSION_OPERATION(VecIndex, ThisVec, Operation, Other) ThisVec[VecIndex] Operation Other[VecIndex];
#include "DefineVectorOps.inl"

// Add another vector
template<> inline
NUMVEC NUMVEC::operator+(const NUMVEC &vOther) const {
  NUMVEC v(*this);
  DO_VECTOR_OPERATION(v, +=, vOther);
  return v;
};

template<> inline
NUMVEC &NUMVEC::operator+=(const NUMVEC &vOther) {
  DO_VECTOR_OPERATION(_values, +=, vOther);
  return *this;
};

// Subtract another vector
template<> inline
NUMVEC NUMVEC::operator-(const NUMVEC &vOther) const {
  NUMVEC v(*this);
  DO_VECTOR_OPERATION(v, -=, vOther);
  return v;
};

template<> inline
NUMVEC &NUMVEC::operator-=(const NUMVEC &vOther) {
  DO_VECTOR_OPERATION(_values, -=, vOther);
  return *this;
};

// Multiply by another vector
template<> inline
NUMVEC NUMVEC::operator*(const NUMVEC &vOther) const {
  NUMVEC v(*this);
  DO_VECTOR_OPERATION(v, *=, vOther);
  return v;
};

template<> inline
NUMVEC &NUMVEC::operator*=(const NUMVEC &vOther) {
  DO_VECTOR_OPERATION(_values, *=, vOther);
  return *this;
};

// Divide by another vector
template<> inline
NUMVEC NUMVEC::operator/(const NUMVEC &vOther) const {
  NUMVEC v(*this);
  DO_VECTOR_OPERATION(v, /=, vOther);
  return v;
};

template<> inline
NUMVEC &NUMVEC::operator/=(const NUMVEC &vOther) {
  DO_VECTOR_OPERATION(_values, /=, vOther);
  return *this;
};

// Negate the vector
template<> inline
NUMVEC NUMVEC::operator-(void) const {
  NUMVEC v(*this);
  DO_VECTOR_OPERATION(v, =, -v);
  return v;
};

#include "UndefVectorOps.inl"
