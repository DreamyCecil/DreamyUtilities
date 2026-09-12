//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_CRC32HASHING_H
#define _DREAMYUTILITIES_INCL_CRC32HASHING_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

#include "HasherBase.hpp"

NAMESPACE_DREAMY_OPEN

// CRC32 hash calculator
class CCRC32Hasher : public IHasher {

protected:
  u32 m_iResult;

public:
  // Default constructor
  CCRC32Hasher();

  // Reset hasher state
  virtual void Reset(void);

  // Hash an array of bytes (adds up to the current hash)
  virtual void AddData(const c8 *pData, size_t iSize);

  // Get hash value as a sequence of bytes
  virtual CByteArray GetBytes(void) const;

  // Get resulting hash
  u32 GetResult(void) const;

public:
  // Get CRC32 value of an array of bytes
  u32 operator()(const c8 *pData, size_t iSize);

  // Begin CRC32 calculation from an existing hash value
  void Begin(u32 iHash = 0);

  // Finish CRC32 calculation
  void Finish(void);
};

NAMESPACE_DREAMY_CLOSE

#endif // (Dreamy Utilities Include Guard)
