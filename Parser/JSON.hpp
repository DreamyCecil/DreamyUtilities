//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_JSON_H
#define _DREAMYUTILITIES_INCL_JSON_H

#include "../DreamyUtilitiesBase.hpp"

#include "Token.hpp"
#include "../Types/Variant.hpp"

namespace dreamy {

// Default JSON constants created at runtime
struct JsonConstants {
  CValObject list;

  // Default constructor
  inline JsonConstants(void) {
    list["null"] = CVariant();
    list["true"] = true;
    list["false"] = false;
  };
};

extern const JsonConstants _jsonConstants;

// Tokenize JSON file contents
void TokenizeJSON(CTokenList &aTokens, const CString &strJSON, const CValObject &oConstants = _jsonConstants.list);

// Build a JSON array
void BuildJSONArray(CVariant &aArray, CTokenList::const_iterator &itCurrent, CTokenList::const_iterator itEnd);

// Build a JSON object
void BuildJSONObject(CVariant &valObject, const CTokenList &aTokens, CTokenList::const_iterator &it);

// Build one value
void BuildJSONValue(CVariant &val, const CTokenList &aTokens, CTokenList::const_iterator &it);

// Build one key-value pair
void BuildJSONPair(CValPair &pair, const CTokenList &aTokens, CTokenList::const_iterator &it);

// Build a tree of values from a tokenized JSON file
void BuildJSON(CVariant &valJSON, const CTokenList &aTokens);

// Parse JSON string and output it in a variant with optional token list
void ParseJSON(CVariant &valJSON, CTokenList *paTokens, const CString &strJSON, const CValObject &oConstants = _jsonConstants.list);

};

#endif // (Dreamy Utilities Include Guard)
