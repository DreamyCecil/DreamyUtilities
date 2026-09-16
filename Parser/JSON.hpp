//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

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
