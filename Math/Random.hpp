//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_MATH_RANDOM_H
#define _DREAMYUTILITIES_INCL_MATH_RANDOM_H

#include "../Base/Base.hpp"

#include <cstdlib>

namespace dreamy
{
  // Get random number between 0 and 1 (inclusive)
  inline f64 RandRatio(void) {
    return static_cast<f64>(rand()) / static_cast<f64>(RAND_MAX);
  };

  // Get random number between min and max (inclusive)
  // Use more efficient "min + rand() % (max + 1)" for integers!
  inline f64 RandRange(const f64 min, const f64 max) {
    return min + static_cast<f64>(rand()) / (static_cast<f64>(RAND_MAX) / (max - min));
  };

};

#endif // (Dreamy Utilities Include Guard)
