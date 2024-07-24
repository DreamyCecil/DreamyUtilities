//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_JSON_H
#define _DREAMYUTILITIES_INCL_JSON_H

#include "../Base/Base.hpp"

// All JSON headers
#include "../JSON/Builder.hpp"
#include "../JSON/Tokenizer.hpp"

namespace dreamy {

// Parse JSON string and output it in a variant with optional token list
inline void ParseJSON(CVariant &valJSON, CTokenList *paTokens, const CString &strJSON, const CValObject &oConstants = _jsonConstants.list) {
  static CTokenList aTokenList;

  // Supply local token list if none specified
  if (paTokens == nullptr) {
    aTokenList.clear();
    paTokens = &aTokenList;
  }

  // Tokenize JSON string and build a value out of it
  TokenizeJSON(*paTokens, strJSON, oConstants);
  BuildJSON(valJSON, *paTokens);
};

};

#endif // (Dreamy Utilities Include Guard)
