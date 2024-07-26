//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_SIMPLEHASHING_H
#define _DREAMYUTILITIES_INCL_SIMPLEHASHING_H

#include "../DreamyUtilitiesBase.hpp"

#include "HasherBase.hpp"

namespace dreamy {

// Simple 64-bit hash calculator
class CSimpleHasher : public IHasher {

protected:
  u64 _result;
  u64 _factor; // Multiplier for hash uniqueness

public:
  // Constructor with a hash factor (use prime numbers like 31 and 37)
  CSimpleHasher(u64 iSetFactor = 31);

  // Reset hasher state
  virtual void Reset(void);

  // Hash an array of bytes (adds up to the current hash)
  virtual void AddData(const c8 *pData, size_t iSize);

  // Get hash value as a sequence of bytes
  virtual CByteArray GetBytes(void) const;

  // Get resulting hash
  u64 GetResult(void) const;

  // Get hash factor
  u64 GetFactor(void) const;

  // Set hash factor
  void SetFactor(u64 iSetFactor);

public:
  // Get hash from an array of bytes
  u64 operator()(const c8 *pData, size_t iSize);
};

};

#endif // (Dreamy Utilities Include Guard)
