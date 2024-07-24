//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_HASHEDSTRING_H
#define _DREAMYUTILITIES_INCL_HASHEDSTRING_H

#include "../Base/Base.hpp"

#include "../Hashing/SimpleHasher.hpp"

namespace dreamy {

// String with its hash value
class CHashedString {

private:
  u64 _hash;
  CString _string;

public:
  // Default constructor
  __forceinline CHashedString() : _hash(0), _string("")
  {
  };

  // Generate a hash value from a string
  __forceinline CHashedString(const CString &str) {
    _string = str;

    CSimpleHasher hasher;
    _hash = hasher(str.c_str(), str.length());
  };

  // Generate a hash value from an array of characters
  __forceinline CHashedString(const c8 *str) {
    _string = str;

    CSimpleHasher hasher;
    _hash = hasher(str, strlen(str));
  };

  // Copy constuctor
  __forceinline CHashedString(const CHashedString &hsOther) :
    _hash(hsOther.GetHash()), _string(hsOther.GetString())
  {
  };

  // Get pure string
  inline const CString &GetString(void) const {
    return _string;
  };

  // Get hash value
  inline u64 GetHash(void) const {
    return _hash;
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
  inline CHashedString operator=(const CHashedString &hsOther) {
    _hash = hsOther.GetHash();
    _string = hsOther.GetString();

    return *this;
  };
};

};

#endif // (Dreamy Utilities Include Guard)
