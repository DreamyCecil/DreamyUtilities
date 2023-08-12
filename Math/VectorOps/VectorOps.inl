//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#if !defined(NUMVEC_OPS_FOR)
  #error Please define amount of dimensions for 'NUMVEC_OPS_FOR' before operation definitions.

#elif !defined(VECTOR_TYPE)
  #error Please define the type for 'VECTOR_TYPE' before operation definitions.
#endif

#include "../Math/VectorOps/VectorOnVector.inl"
#include "../Math/VectorOps/VectorOnFactor.inl"

// Define operations for adding up vector values
#define DO_DIMENSION_OPERATION(VecIndex, ThisVec, Operation, Other) +(ThisVec[VecIndex] Operation Other[VecIndex])
#include "../Math/VectorOps/OpsDefine.inl"

// Dot product between two vectors
template<> inline
VECTOR_TYPE NUMVEC::operator%(const NUMVEC &vOther) const {
  // Compiles as "total = + (this[0] * other[0]) + (this[1] * other[1]) ..."
  VECTOR_TYPE total =  DO_VECTOR_OPERATION(_values, *, vOther);
  return total;
};

// Get length of the vector
template<> inline
VECTOR_TYPE NUMVEC::Length(void) const {
  VECTOR_TYPE total = (*this) % (*this);
  return (VECTOR_TYPE)sqrt(total);
};

#include "../Math/VectorOps/OpsUndef.inl"

#undef NUMVEC_OPS_FOR
