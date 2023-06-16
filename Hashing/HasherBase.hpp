//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_HASHERBASE_H
#define _DREAMYUTILITIES_INCL_HASHERBASE_H

#include "../Base/Base.hpp"

#include "../Data/ByteArray.hpp"

namespace dreamy {

class IHasher {

public:
  // Reset hasher state
  virtual void Reset(void) = 0;

  // Hash an array of bytes (adds up to the current hash)
  virtual void AddData(const c8 *pData, size_t iSize) = 0;

  // Get hash value as a sequence of bytes
  virtual CByteArray GetBytes(void) const = 0;
};

};

#endif // (Dreamy Utilities Include Guard)
