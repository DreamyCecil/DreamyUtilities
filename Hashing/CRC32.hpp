/* Copyright (c) 2022-2026 Dreamy Cecil
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
