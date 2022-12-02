//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_SIMPLEHASHING_H
#define _DREAMYUTILITIES_INCL_SIMPLEHASHING_H

#include "../Base/Base.hpp"

#include "HasherBase.hpp"

namespace dreamy
{
  // Simple 64-bit hash calculator
  class CSimpleHasher : public IHasher {
    protected:
      u64 _result;
      u64 _factor; // Multiplier for hash uniqueness

    public:
      // Constructor with a hash factor (use prime numbers like 31 and 37)
      CSimpleHasher(u64 iSetFactor = 31) : _result(0), _factor(iSetFactor)
      {
      };

      // Reset hasher state
      virtual void Reset(void) {
        _result = 0;
      };

      // Hash an array of bytes (adds up to the current hash)
      virtual void AddData(const c8 *pData, size_t iSize)
      {
        for (size_t i = 0; i < iSize; ++i) {
          _result = _factor * _result + pData[i];
        }
      };

      // Get hash value as a sequence of bytes
      virtual CByteArray GetBytes(void) const
      {
        return CByteArray(reinterpret_cast<const c8 *>(&_result), sizeof(_result));
      };

      // Get resulting hash
      inline u64 GetResult(void) const {
        return _result;
      };

      // Get hash factor
      inline u64 GetFactor(void) const {
        return _factor;
      };

      // Set hash factor
      inline void SetFactor(u64 iSetFactor) {
        _factor = iSetFactor;
      };

    public:
      // Get hash from an array of bytes
      inline u64 operator()(const c8 *pData, size_t iSize) {
        Reset();
        AddData(pData, iSize);

        return _result;
      };
  };

};

#endif // (Dreamy Utilities Include Guard)
