//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

// Define operations for vectors with factors
#define DO_DIMENSION_OPERATION(VecIndex, ThisVec, Operation, Other) ThisVec[VecIndex] Operation Other;
#include "DefineVectorOps.inl"

// Multiply vector by a factor
template<> inline
NUMVEC NUMVEC::operator*(const VECTOR_TYPE factor) const {
  NUMVEC v(*this);
  DO_VECTOR_OPERATION(v, *=, factor);
  return v;
};

template<> inline
NUMVEC &NUMVEC::operator*=(const VECTOR_TYPE factor) {
  DO_VECTOR_OPERATION(_values, *=, factor);
  return *this;
};

// Divide vector by a factor
template<> inline
NUMVEC NUMVEC::operator/(const VECTOR_TYPE factor) const {
  NUMVEC v(*this);
  DO_VECTOR_OPERATION(v, /=, factor);
  return v;
};

template<> inline
NUMVEC &NUMVEC::operator/=(const VECTOR_TYPE factor) {
  DO_VECTOR_OPERATION(_values, /=, factor);
  return *this;
};

#include "UndefVectorOps.inl"
