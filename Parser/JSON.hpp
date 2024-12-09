//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_JSON_H
#define _DREAMYUTILITIES_INCL_JSON_H

#include "../DreamyUtilitiesBase.hpp"

#include "Token.hpp"
#include "../Types/Variant.hpp"

namespace dreamy {

namespace json {

// Default JSON constants created at runtime
struct Constants {
  CValObject list;

  // Default constructor
  inline Constants(void) {
    list["null"] = CVariant();
    list["true"] = true;
    list["false"] = false;
  };
};

extern const Constants _constants;

// Tokenize JSON file contents
void Tokenize(CTokenList &aTokens, const CString &strJSON, const CValObject &oConstants = _constants.list);

// Build a JSON array
void BuildArray(CVariant &aArray, CTokenList::const_iterator &itCurrent, CTokenList::const_iterator itEnd);

// Build a JSON object
void BuildObject(CVariant &valObject, const CTokenList &aTokens, CTokenList::const_iterator &it);

// Build one value
void BuildValue(CVariant &val, const CTokenList &aTokens, CTokenList::const_iterator &it);

// Build one key-value pair
void BuildPair(CValPair &pair, const CTokenList &aTokens, CTokenList::const_iterator &it);

// Build a tree of values from a tokenized JSON file
void Build(CVariant &valJSON, const CTokenList &aTokens);

// Parse JSON string and output it in a variant with optional token list
void Parse(CVariant &valJSON, CTokenList *paTokens, const CString &strJSON, const CValObject &oConstants = _constants.list);

}; // namespace json

}; // namespace dreamy

#endif // (Dreamy Utilities Include Guard)
