//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

// Define operations for degree to radian conversion
#define DO_DIMENSION_OPERATION(VecIndex, ThisVec, Operation, Other) ThisVec[VecIndex] Operation dreamy::DegToRad(Other[VecIndex]);
#include "OpsDefine.inl"

// Convert values from degrees to radians
template<> inline
NUMVEC NUMVEC::DegToRad(void) const {
  NUMVEC v(*this);
  DO_VECTOR_OPERATION(v, =, _values);
  return v;
};

#include "OpsUndef.inl"

// Define operations for radian to degree conversion
#define DO_DIMENSION_OPERATION(VecIndex, ThisVec, Operation, Other) ThisVec[VecIndex] Operation dreamy::RadToDeg(Other[VecIndex]);
#include "OpsDefine.inl"

// Convert values from radians to degrees
template<> inline
NUMVEC NUMVEC::RadToDeg(void) const {
  NUMVEC v(*this);
  DO_VECTOR_OPERATION(v, =, _values);
  return v;
};

#include "OpsUndef.inl"
