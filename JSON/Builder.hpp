//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_JSON_BUILDER_H
#define _DREAMYUTILITIES_INCL_JSON_BUILDER_H

#include "../Base/Base.hpp"

#include "../Parser/Token.hpp"

namespace dreamy
{
  inline void BuildJSONPair(CValPair &pair, const CTokenList &aTokens, CTokenList::const_iterator &it);

  // Build a JSON array
  inline void BuildJSONArray(CVariant &aArray, CTokenList::const_iterator &itCurrent, CTokenList::const_iterator itEnd) {
    CTokenList::const_iterator itClosing = std::find(itCurrent, itEnd, CParserToken(CParserToken::TKN_GROUP_CLOSE));

    // Unclosed array
    if (itClosing == itEnd) {
      throw CTokenException(itCurrent->GetTokenPos(), "Unclosed array");
    }

    CValArray aValues;
    bool bNext = false; // Search for the next array entry

    while (itCurrent != itClosing) {
      const CParserToken &tkn = (*itCurrent)(bNext ? CParserToken::TKN_COMMA : CParserToken::TKN_VALUE);

      // Add one value
      if (!bNext) {
        aValues.push_back(tkn.GetValue());
      }

      bNext = !bNext;
      ++itCurrent;
    }

    aArray.FromArray(aValues);
  };

  // Build a JSON object
  inline void BuildJSONObject(CVariant &valObject, const CTokenList &aTokens, CTokenList::const_iterator &it) {
    const CTokenList::const_iterator itStart = it;

    CValObject oValues;
    bool bNext = false; // Search for the next array entry

    while (it != aTokens.end()) {
      // The next token should be a key string
      if (!bNext) {
        // Add one key-value pair
        CValPair pair;
        BuildJSONPair(pair, aTokens, it);

        oValues.insert(pair);
        ++it;

      // The next token should be a comma
      } else {
        (*(it++))(CParserToken::TKN_COMMA);

        // Reached the block end right after a comma
        if (it->GetType() == CParserToken::TKN_BLOCK_CLOSE) {
          break;
        }
      }

      bNext = !bNext;
    }

    // Unclosed object
    if (it == aTokens.end()) {
      throw CTokenException(itStart->GetTokenPos(), "Unclosed object");
    }

    valObject.FromObject(oValues);
  };

  // Build one value
  inline void BuildJSONValue(CVariant &val, const CTokenList &aTokens, CTokenList::const_iterator &it) {
    const u32 iToken = it->GetType();

    switch (iToken) {
      // Object with variables {}
      case CParserToken::TKN_BLOCK_OPEN:
        BuildJSONObject(val, aTokens, ++it);
        return;

      // Array of values []
      case CParserToken::TKN_GROUP_OPEN:
        BuildJSONArray(val, ++it, aTokens.end());
        return;

      // Pure value
      case CParserToken::TKN_VALUE:
        val = it->GetValue();
        return;

      // Value beginning with a unary operator
      case CParserToken::TKN_ADD:
      case CParserToken::TKN_SUB: {
        s8 iSign = (iToken == CParserToken::TKN_SUB ? -1 : 1);

        switch (it->GetValue().GetType()) {
          case CVariant::VAL_S64: {
            s64 iNumber = it->GetValue().ToS64();
            val.FromS64(iNumber * iSign);
          }

          case CVariant::VAL_F64: {
            f64 fNumber = it->GetValue().ToF64();
            val.FromF64(fNumber * iSign);
          }

          // Not a number
          default:
            throw CTokenException(it->GetTokenPos(), "Expected a number after the unary operator");
        }
      }

      // Invalid token
      default:
        throw CTokenException(it->GetTokenPos(), "Expected an object, an array or a value");
    }
  };

  void BuildJSONPair(CValPair &pair, const CTokenList &aTokens, CTokenList::const_iterator &it) {
    // Key name ("key")
    const CParserToken &tknKey = (*(it++))(CParserToken::TKN_VALUE);

    // Not a string
    if (tknKey.GetValue().GetType() != CVariant::VAL_STR) {
      throw CTokenException(it->GetTokenPos(), "Expected a name string");
    }

    // Key assignment ("key" : )
    (*(it++))(CParserToken::TKN_COLON);

    // Build the value and make the pair
    CHashedString hsKey(tknKey.GetValue().ToString());
    CVariant val;
    BuildJSONValue(val, aTokens, it);

    pair = std::make_pair(hsKey, val);
  };

  // Build a tree of values from a tokenized JSON file
  inline void BuildJSON(CVariant &valJSON, const CTokenList &aTokens) {
    // No tokens
    if (aTokens.size() == 0) {
      valJSON = CVariant();
      return;
    }

    CTokenList::const_iterator it = aTokens.begin();
    BuildJSONValue(valJSON, aTokens, it);
  };

};

#endif // (Dreamy Utilities Include Guard)
