//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#if !defined(NUMVEC_OPS_FOR)
  #error Please define amount of dimensions for 'NUMVEC_OPS_FOR' before operation definitions.

#elif !defined(VECTOR_TYPE)
  #error Please define the type for 'VECTOR_TYPE' before operation definitions.
#endif

#include "../Math/VectorOps/DegRad.inl"

#undef NUMVEC_OPS_FOR
