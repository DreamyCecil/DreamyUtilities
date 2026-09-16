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

#ifndef _DREAMYUTILITIES_INCL_JSON_H
#define _DREAMYUTILITIES_INCL_JSON_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

#include "Token.hpp"
#include "../Types/Variant.hpp"

NAMESPACE_DREAMY_OPEN

namespace json5 {

// Default JSON constants
struct Constants {
  // No constants
  static __forceinline CDictionary None(void) {
    return CDictionary();
  };

  // Only null
  static __forceinline CDictionary Null(void) {
    CDictionary dict;
    dict["null"] = CVariant();
    return dict;
  };

  // Only boolean constants
  static __forceinline CDictionary Boolean(void) {
    CDictionary dict;
    dict["true"] = true;
    dict["false"] = false;
    return dict;
  };

  // Default JSON constants
  static __forceinline CDictionary JSON(void) {
    CDictionary dict;
    dict["null"] = CVariant();
    dict["true"] = true;
    dict["false"] = false;
    return dict;
  };

  // Default JSON5 constants
  static __forceinline CDictionary Default(void) {
    CDictionary dict;
    dict["null"] = CVariant();
    dict["true"] = true;
    dict["false"] = false;
    dict["Infinity"] = dreamy::math::Infinity;
    dict["NaN"] = dreamy::math::NaN;
    return dict;
  };
};

// Tokenize JSON file contents
void Tokenize(CTokenList &aTokens, const CString &strJSON);

// Build a JSON array
void BuildArray(CVariant &valArray, const CTokenList &aTokens, CTokenList::const_iterator &it,
  const CDictionary &dictConstants = Constants::Default());

// Build a JSON object
void BuildObject(CVariant &valObject, const CTokenList &aTokens, CTokenList::const_iterator &it,
  const CDictionary &dictConstants = Constants::Default());

// Build one value
void BuildValue(CVariant &val, const CTokenList &aTokens, CTokenList::const_iterator &it,
  const CDictionary &dictConstants = Constants::Default());

// Build one key-value pair
void BuildPair(CPair &pair, const CTokenList &aTokens, CTokenList::const_iterator &it,
  const CDictionary &dictConstants = Constants::Default());

// Build a tree of values from a tokenized JSON file
void Build(CVariant &valJSON, const CTokenList &aTokens,
  const CDictionary &dictConstants = Constants::Default());

// Parse JSON string and output it in a variant with optional token list
void Parse(CVariant &valJSON, CTokenList *paTokens, const CString &strJSON,
  const CDictionary &dictConstants = Constants::Default());

}; // namespace json

NAMESPACE_DREAMY_CLOSE

#endif // (Dreamy Utilities Include Guard)
