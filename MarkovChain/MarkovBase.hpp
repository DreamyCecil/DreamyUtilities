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

#ifndef _DREAMYUTILITIES_INCL_MARKOV_BASE_H
#define _DREAMYUTILITIES_INCL_MARKOV_BASE_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

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
