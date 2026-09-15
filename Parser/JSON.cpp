//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#include "JSON.hpp"

#include "../Parser/CLikeParser.hpp"

NAMESPACE_DREAMY_OPEN

namespace json {

// Default JSON constants created at runtime
const Constants _constants;

// Tokenize JSON file contents
void Tokenize(CTokenList &aTokens, const CString &strJSON, const CDictionary &dictConstants) {
  CCLikeParser data(strJSON);

  DREAMY_PARSE_FOR(data) {
    switch (data.Cur()) {
      // Skip spaces
      case ' ': case '\t': case '\r': {
        data.Advance(1);
      } break;

      // Line break
      case '\n': {
        data.Advance(1);
        data.CountLine();
      } break;

      case ':': // Key-value assignment
      case ',': // Next value
      case '{': case '}': // Object block
      case '[': case ']': // Array block
      case '+': case '-': { // Unary operators
        AddToken(aTokens, data.Cur(), data.GetTokenPos(), 0);
        data.Advance(1);
      } break;

      default: {
        // Keywords
        if (data.TokenizeKey(&aTokens)) {
          // Assume it's an identifier
          CParserToken &tkn = aTokens[aTokens.size() - 1];
          const CVariant &valIdentifier = tkn.GetValue();

          // Find constant in the list and retrieve its value
          CDictionary::const_iterator itConst = dictConstants.find(valIdentifier);

          if (itConst != dictConstants.end()) {
            tkn = CParserToken(CParserToken::TKN_VALUE, tkn.GetTokenPos(), itConst->second);
          } else {
            CTokenException::Throw(data.GetTokenPos(), "Invalid constant '%s'", valIdentifier.AsString().c_str());
          }

        // Special tokenizers
        } else {
          bool bTokenized = data.ParseComment()
            || data.TokenizeNumber(&aTokens)
            || data.TokenizeCharSequence('\"', '\'', &aTokens);

          if (!bTokenized) {
            throw CTokenException(data.GetTokenPos(), "Invalid character for tokenization");
          }
        }
      } break;
    }
  }
};

// Build a JSON array
void BuildArray(CVariant &valArray, const CTokenList &aTokens, CTokenList::const_iterator &it) {
  const CTokenList::const_iterator itStart = it;

  // Create an empty array beforehand and retrieve it
  valArray.FromArray(CArray());
  CArray &aValues = valArray.AsArray();

  // Array opening
  (it++)->Verify('[');

  // Empty array
  if (it->GetType() == ']') {
    ++it;
    return;
  }

  bool bNext = false; // Search for the next array entry

  while (it != aTokens.end()) {
    // The next token should be a value
    if (!bNext) {
      // Add one value
      CVariant val;
      BuildValue(val, aTokens, it);
      aValues.push_back(val);

    // The next token could be a comma
    } else if (it->GetType() == ',') {
      (it++)->Verify(',');

      // Reached the array end right after an optional comma
      if (it->GetType() == ']') {
        ++it;
        return;
      }

    // If not, expect the array end
    } else {
      (it++)->Verify(']');
      return;
    }

    bNext = !bNext;
  }

  // Unclosed array
  if (it == aTokens.end()) {
    throw CTokenException(itStart->GetTokenPos(), "Unclosed array");
  }
};

// Build a JSON object
void BuildObject(CVariant &valObject, const CTokenList &aTokens, CTokenList::const_iterator &it) {
  const CTokenList::const_iterator itStart = it;

  // Create an empty dictionary beforehand and retrieve it
  valObject.FromDictionary(CDictionary());
  CDictionary &dictValues = valObject.AsDictionary();

  // Object opening
  (it++)->Verify('{');

  // Empty object
  if (it->GetType() == '}') {
    ++it;
    return;
  }

  bool bNext = false; // Search for the next object entry

  while (it != aTokens.end()) {
    // The next token should be a key string
    if (!bNext) {
      // Add one key-value pair
      CPair pair;
      BuildPair(pair, aTokens, it);
      dictValues.insert(pair);

    // The next token could be a comma
    } else if (it->GetType() == ',') {
      (it++)->Verify(',');

      // Reached the object end right after an optional comma
      if (it->GetType() == '}') {
        ++it;
        return;
      }

    // If not, expect the object end
    } else {
      (it++)->Verify('}');
      return;
    }

    bNext = !bNext;
  }

  // Unclosed object
  if (it == aTokens.end()) {
    throw CTokenException(itStart->GetTokenPos(), "Unclosed object");
  }
};

// Build one value
void BuildValue(CVariant &val, const CTokenList &aTokens, CTokenList::const_iterator &it) {
  const u32 iToken = it->GetType();

  switch (iToken) {
    // Object with variables {}
    case '{': {
      BuildObject(val, aTokens, it);
    } return;

    // Array of values []
    case '[': {
      BuildArray(val, aTokens, it);
    } return;

    // Pure value
    case CParserToken::TKN_VALUE: {
      val = it->GetValue();
      ++it;
    } return;

    // Value beginning with a unary operator
    case '+': case '-': {
      (++it)->Verify(CParserToken::TKN_VALUE);

      switch (it->GetValue().GetType()) {
        case CVariant::VAL_INT: {
          s64 iNumber = it->GetValue().AsInt();
          val.FromInt(iToken == '-' ? -iNumber : iNumber);
        } break;

        case CVariant::VAL_FLOAT: {
          f64 fNumber = it->GetValue().AsFloat();
          val.FromFloat(iToken == '-' ? -fNumber : fNumber);
        } break;

        // Not a number
        default:
          throw CTokenException(it->GetTokenPos(), "Expected a number after the unary operator");
      }

      ++it;
    } return;

    // Invalid token
    default:
      throw CTokenException(it->GetTokenPos(), "Expected an object, an array or a value");
  }
};

// Build one key-value pair
void BuildPair(CPair &pair, const CTokenList &aTokens, CTokenList::const_iterator &it) {
  const CParserToken &tknKey = *(it++);

  // Expect a name (e.g. "key")
  if (tknKey.GetType() == CParserToken::TKN_VALUE) {
    // Not a string
    if (tknKey.GetValue().GetType() != CVariant::VAL_STRING) {
      throw CTokenException(it->GetTokenPos(), "Expected a name string or identifier");
    }

  // Expect an identifier (e.g. key)
  } else if (tknKey.GetType() != CParserToken::TKN_KEY) {
    throw CTokenException(it->GetTokenPos(), "Expected a name string or identifier");
  }

  // Key assignment ("key" : )
  (it++)->Verify(':');

  // Build the value and make the pair
  CVariant val;
  BuildValue(val, aTokens, it);

  pair = std::make_pair(tknKey.GetValue(), val);
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
void Parse(CVariant &valJSON, CTokenList *paTokens, const CString &strJSON, const CDictionary &dictConstants) {
  static CTokenList aTokenList;

  // Supply local token list if none specified
  if (paTokens == nullptr) {
    aTokenList.clear();
    paTokens = &aTokenList;
  }

  // Tokenize JSON string and build a value out of it
  Tokenize(*paTokens, strJSON, dictConstants);
  Build(valJSON, *paTokens);
};

}; // namespace json

NAMESPACE_DREAMY_CLOSE
