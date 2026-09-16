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

#ifndef _DREAMYUTILITIES_INCL_HASHEDSTRING_H
#define _DREAMYUTILITIES_INCL_HASHEDSTRING_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

#include "../Hashing/SimpleHasher.hpp"

NAMESPACE_DREAMY_OPEN

// String with its hash value
class CHashedString {

private:
  u64 m_iHash;
  CString m_strValue;

public:
  // Default constructor
  __forceinline CHashedString() : m_iHash(0), m_strValue("")
  {
  };

  // Generate a hash value from a string
  __forceinline CHashedString(const CString &str) {
    m_strValue = str;

    CSimpleHasher hasher;
    m_iHash = hasher(str.c_str(), str.length());
  };

  // Generate a hash value from an array of characters
  __forceinline CHashedString(const c8 *str) {
    m_strValue = str;

    CSimpleHasher hasher;
    m_iHash = hasher(str, strlen(str));
  };

  // Copy constuctor
  __forceinline CHashedString(const CHashedString &hsOther) :
    m_iHash(hsOther.GetHash()), m_strValue(hsOther.GetString())
  {
  };

  // Get pure string
  inline const CString &GetString(void) const {
    return m_strValue;
  };

  // Get hash value
  inline u64 GetHash(void) const {
    return m_iHash;
  };

public:

  // Quick comparison via hashes
  inline bool operator==(const CHashedString &hsOther) const {
    return GetHash() == hsOther.GetHash();
  };

  // Comparison operator for sorting (in alphabetical order)
  inline bool operator<(const CHashedString &hsOther) const {
    return GetString() < hsOther.GetString();
  };

  // Assignment operator
  inline CHashedString &operator=(const CHashedString &hsOther) {
    m_iHash = hsOther.GetHash();
    m_strValue = hsOther.GetString();

    return *this;
  };
};

NAMESPACE_DREAMY_CLOSE

#endif // (Dreamy Utilities Include Guard)
