//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_MATH_ALGORITHM_H
#define _DREAMYUTILITIES_INCL_MATH_ALGORITHM_H

#include "../Base/Base.hpp"

namespace dreamy {

namespace math {

#define MATH_TEMP template<typename Type>

// Return smaller of two values
template<typename Type1, typename Type2> __forceinline
Type1 Min(Type1 x, Type2 y) {
  return (x < y) ? x : y;
};

// Return larger of two values
template<typename Type1, typename Type2> __forceinline
Type1 Max(Type1 x, Type2 y) {
  return (x < y) ? y : x;
};

// Clamp value between certain boundaries
template<typename Type1, typename Type2, typename Type3> __forceinline
Type1 Clamp(Type1 x, Type2 min, Type3 max) {
  return (x < min) ? min : (max < x) ? max : x;
};

#undef MATH_TEMP

};

};

#endif // (Dreamy Utilities Include Guard)
