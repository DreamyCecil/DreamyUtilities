//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_MARKOV_BASE_H
#define _DREAMYUTILITIES_INCL_MARKOV_BASE_H

#include "../Base/Base.hpp"

#include <vector>

// Hash factor
#include "../Types/HashedString.hpp"

#if _DREAMY_CPP11
  namespace std
  {
    // Standard vector hasher
    template<typename Type>
    struct hash<vector<Type>>
    {
      size_t operator()(const vector<Type> &a) const {
        hash<Type> hasher;
        size_t hash = 0;

        const size_t iSize = a.size();

        for (size_t i = 0; i < iSize; ++i) {
          hash = hash * 31 + hasher(a[i]);
        }

        return hash;
      }
    };
  };

  // Map of unique orders
  #include <unordered_map>

#else
  // Map of unique orders
  #include "../Types/UnorderedMap.hpp"

#endif

#endif // (Dreamy Utilities Include Guard)
