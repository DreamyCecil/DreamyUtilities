//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

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
