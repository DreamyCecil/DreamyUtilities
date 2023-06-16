//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_MATH_ALGORITHM_H
#define _DREAMYUTILITIES_INCL_MATH_ALGORITHM_H

#include "../Base/Base.hpp"

namespace dreamy {

namespace math {

#define MATH_TEMP template<typename Type>

// Return smaller of two values
MATH_TEMP inline Type Min(Type x, Type y) {
  return (x < y) ? x : y;
};

// Return larger of two values
MATH_TEMP inline Type Max(Type x, Type y) {
  return (x < y) ? y : x;
};

// Clamp value between certain boundaries
MATH_TEMP inline Type Clamp(Type x, Type min, Type max) {
  return (x < min) ? min : (max < x) ? max : x;
};

#undef MATH_TEMP

};

};

#endif // (Dreamy Utilities Include Guard)
