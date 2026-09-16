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

#include "JSON.hpp"

#include "../Parser/CLikeParser.hpp"

NAMESPACE_DREAMY_OPEN

namespace json5 {

// Tokenize JSON file contents
void Tokenize(CTokenList &aTokens, const CString &strJSON) {
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

      // String literals (character literals not supported)
      case '\"': case '\'': {
        CString str;
        if (data.ParseString(data.Cur(), &str)) {
          AddToken(aTokens, CParserToken::TKN_VALUE, data.GetTokenPos(), str);
        } else {
          // Normally, ParseString() throws exceptions on any error, so this shouldn't happen
          throw CTokenException(data.GetTokenPos(), "Invalid string literal");
        }
      } break;

      default: {
        // Special tokenizers
        bool bTokenized = data.ParseComment() || data.TokenizeKey(&aTokens) || data.TokenizeNumber(&aTokens);

        if (!bTokenized) {
          throw CTokenException(data.GetTokenPos(), "Invalid character for tokenization");
        }
      } break;
    }
  }
};

// Build a JSON array
void BuildArray(CVariant &valArray, const CTokenList &aTokens, CTokenList::const_iterator &it, const CDictionary &dictConstants) {
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
      BuildValue(val, aTokens, it, dictConstants);
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
void BuildObject(CVariant &valObject, const CTokenList &aTokens, CTokenList::const_iterator &it, const CDictionary &dictConstants) {
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
      BuildPair(pair, aTokens, it, dictConstants);
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
void BuildValue(CVariant &val, const CTokenList &aTokens, CTokenList::const_iterator &it, const CDictionary &dictConstants) {
  const u32 iToken = it->GetType();

  switch (iToken) {
    // Object with variables {}
    case '{': {
      BuildObject(val, aTokens, it, dictConstants);
    } return;

    // Array of values []
    case '[': {
      BuildArray(val, aTokens, it, dictConstants);
    } return;

    // Constant
    case CParserToken::TKN_KEY: {
      // Find constant in the list and retrieve its value
      const CVariant &valIdentifier = it->GetValue();
      CDictionary::const_iterator itConst = dictConstants.find(valIdentifier);

      if (itConst == dictConstants.end()) {
        CTokenException::Throw(it->GetTokenPos(), "Invalid constant '%s'", valIdentifier.AsString().c_str());
      }

      val = itConst->second;
      ++it;
    } return;

    // Pure value
    case CParserToken::TKN_VALUE: {
      val = it->GetValue();
      ++it;
    } return;

    // Value beginning with a unary operator
    case '+': case '-': {
      // Interpret the next token as a value
      BuildValue(val, aTokens, ++it, dictConstants);

      // And then change its value if it's numeric
      switch (val.GetType()) {
        case CVariant::VAL_INT: {
          s64 iNumber = val.AsInt();
          val.FromInt(iToken == '-' ? -iNumber : iNumber);
        } break;

        case CVariant::VAL_FLOAT: {
          f64 fNumber = val.AsFloat();
          val.FromFloat(iToken == '-' ? -fNumber : fNumber);
        } break;

        // Not a number
        default:
          throw CTokenException(it->GetTokenPos(), "Expected an integer or a float value after the unary operator");
      }
    } return;

    // Invalid token
    default:
      throw CTokenException(it->GetTokenPos(), "Expected an object, an array or a value");
  }
};

// Build one key-value pair
void BuildPair(CPair &pair, const CTokenList &aTokens, CTokenList::const_iterator &it, const CDictionary &dictConstants) {
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

  // Set the key and build the value for the pair
  pair.first = tknKey.GetValue();
  BuildValue(pair.second, aTokens, it, dictConstants);
};

// Build a tree of values from a tokenized JSON file
void Build(CVariant &valJSON, const CTokenList &aTokens, const CDictionary &dictConstants) {
  // No tokens
  if (aTokens.size() == 0) {
    valJSON = CVariant();
    return;
  }

  CTokenList::const_iterator it = aTokens.begin();
  BuildValue(valJSON, aTokens, it, dictConstants);
};

// Parse JSON string and output it in a variant with optional token list
void Parse(CVariant &valJSON, CTokenList *paTokens, const CString &strJSON, const CDictionary &dictConstants) {
  // Supply local token list if none specified
  CTokenList aTokenList;
  if (paTokens == nullptr) paTokens = &aTokenList;

  // Tokenize JSON string and build a value out of it
  Tokenize(*paTokens, strJSON);
  Build(valJSON, *paTokens, dictConstants);
};

}; // namespace json

NAMESPACE_DREAMY_CLOSE
