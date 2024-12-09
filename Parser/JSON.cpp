//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#include "JSON.hpp"

#include "../Parser/ParserData.hpp"

namespace dreamy {

namespace json {
  
// Default JSON constants created at runtime
const Constants _constants;

// Tokenize JSON file contents
void Tokenize(CTokenList &aTokens, const CString &strJSON, const CValObject &oConstants) {
  CParserData data(strJSON);

  while (data.CanParse()) {
    const c8 ch = *data.pchCur;

    switch (ch) {
      // Skip spaces
      case ' ': case '\t': case '\r': break;

      // Line break
      case '\n': data.CountLine(); break;

      case ':': // Key-value assignment
      case ',': // Next value
      case '{': case '}': // Object block
      case '[': case ']': // Array block
      case '+': case '-': // Unary operators
        AddToken(aTokens, ch, data.pos, ch);
        break;

      default: {
        // Keywords
        if (data.ParseIdentifiers(aTokens)) {
          // Assume it's an identifier
          CParserToken &tkn = aTokens[aTokens.size() - 1];
          const CString &strName = tkn.GetValue().ToString();

          // Find constant in the list and retrieve its value
          CValObject::const_iterator itConst = oConstants.find(strName);

          if (itConst != oConstants.end()) {
            tkn = CParserToken(CParserToken::TKN_VALUE, tkn.GetTokenPos(), itConst->second);
          } else {
            CTokenException::Throw(data.pos, "Invalid constant '%s'", strName.c_str());
          }

        // Special tokenizers
        } else {
          bool bTokenized = data.ParseComments(aTokens, false)
            || data.ParseNumbers(aTokens)
            || data.ParseCharSequences(aTokens, '"', '\'');

          if (!bTokenized) {
            throw CTokenException(data.pos, "Invalid character for tokenization");
          }
        }
      } break;
    }
  }
};

// Build a JSON array
void BuildArray(CVariant &aArray, CTokenList::const_iterator &itCurrent, CTokenList::const_iterator itEnd) {
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
void BuildObject(CVariant &valObject, const CTokenList &aTokens, CTokenList::const_iterator &it) {
  const CTokenList::const_iterator itStart = it;

  CValObject oValues;
  bool bNext = false; // Search for the next array entry

  while (it != aTokens.end()) {
    // The next token should be a key string
    if (!bNext) {
      // Add one key-value pair
      CValPair pair;
      BuildPair(pair, aTokens, it);

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
void BuildValue(CVariant &val, const CTokenList &aTokens, CTokenList::const_iterator &it) {
  const u32 iToken = it->GetType();

  switch (iToken) {
    // Object with variables {}
    case CParserToken::TKN_BLOCK_OPEN:
      BuildObject(val, aTokens, ++it);
      return;

    // Array of values []
    case CParserToken::TKN_GROUP_OPEN:
      BuildArray(val, ++it, aTokens.end());
      return;

    // Pure value
    case CParserToken::TKN_VALUE:
      val = it->GetValue();
      return;

    // Value beginning with a unary operator
    case CParserToken::TKN_ADD:
    case CParserToken::TKN_SUB: {
      switch (it->GetValue().GetType()) {
        case CVariant::VAL_INT: {
          s64 iNumber = it->GetValue().ToInt();
          val.FromInt(iToken == CParserToken::TKN_SUB ? -iNumber : iNumber);
        }

        case CVariant::VAL_FLOAT: {
          f64 fNumber = it->GetValue().ToFloat();
          val.FromFloat(iToken == CParserToken::TKN_SUB ? -fNumber : fNumber);
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

// Build one key-value pair
void BuildPair(CValPair &pair, const CTokenList &aTokens, CTokenList::const_iterator &it) {
  // Key name ("key")
  const CParserToken &tknKey = (*(it++))(CParserToken::TKN_VALUE);

  // Not a string
  if (tknKey.GetValue().GetType() != CVariant::VAL_STRING) {
    throw CTokenException(it->GetTokenPos(), "Expected a name string");
  }

  // Key assignment ("key" : )
  (*(it++))(CParserToken::TKN_COLON);

  // Build the value and make the pair
  CHashedString hsKey(tknKey.GetValue().ToString());
  CVariant val;
  BuildValue(val, aTokens, it);

  pair = std::make_pair(hsKey, val);
};

// Build a tree of values from a tokenized JSON file
void Build(CVariant &valJSON, const CTokenList &aTokens) {
  // No tokens
  if (aTokens.size() == 0) {
    valJSON = CVariant();
    return;
  }

  CTokenList::const_iterator it = aTokens.begin();
  BuildValue(valJSON, aTokens, it);
};

// Parse JSON string and output it in a variant with optional token list
void Parse(CVariant &valJSON, CTokenList *paTokens, const CString &strJSON, const CValObject &oConstants) {
  static CTokenList aTokenList;

  // Supply local token list if none specified
  if (paTokens == nullptr) {
    aTokenList.clear();
    paTokens = &aTokenList;
  }

  // Tokenize JSON string and build a value out of it
  Tokenize(*paTokens, strJSON, oConstants);
  Build(valJSON, *paTokens);
};

}; // namespace json

}; // namespace dreamy
