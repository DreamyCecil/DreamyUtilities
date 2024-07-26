//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_MATH_TRIGONOMETRY_H
#define _DREAMYUTILITIES_INCL_MATH_TRIGONOMETRY_H

#include "../DreamyUtilitiesBase.hpp"

#include "Algebra.hpp"

namespace dreamy {

#define MATH_TEMP template<typename Type>

// Wrap angle to be between 0 and 360 degrees
MATH_TEMP inline Type WrapAngle(Type angle) {
  return (Type)fmod(fmod((f64)angle, 360.0) + 360.0, 360.0);
};

// Normalize angle to be between -180 and +180 degrees
MATH_TEMP inline Type NormalizeAngle(Type angle) {
  return Type(WrapAngle((f64)angle + 180.0) - 180.0);
};

// Convert radians to degrees
MATH_TEMP inline Type RadToDeg(Type radians) {
  return Type((f64)radians * (f64(180.0) / dreamy::math::PI));
};

// Convert degrees to radians
MATH_TEMP inline Type DegToRad(Type angle) {
  return Type(WrapAngle((f64)angle) * (dreamy::math::PI / 180.0));
};

#undef MATH_TEMP

};

#endif // (Dreamy Utilities Include Guard)
