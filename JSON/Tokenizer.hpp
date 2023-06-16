//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_JSON_TOKENIZER_H
#define _DREAMYUTILITIES_INCL_JSON_TOKENIZER_H

#include "../Base/Base.hpp"

#include "../Parser/ParserData.hpp"

namespace dreamy
{
  // Default JSON constants created at runtime
  static const struct JsonConstants {
    CValObject list;

    // Default constructor
    inline JsonConstants(void) {
      CVariant val;
      list["null"] = val;

      val.FromBit(true);
      list["true"] = val;

      val.FromBit(false);
      list["false"] = val;
    };
  } _jsonConstants;

  // Tokenize JSON file contents
  inline void TokenizeJSON(CTokenList &aTokens, const Str_t &strJSON, const CValObject &oConstants = _jsonConstants.list) {
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
          AddIntegerToken(aTokens, ch, data.pos, ch);
          break;

        default: {
          // Keywords
          if (data.ParseIdentifiers(aTokens)) {
            // Assume it's an identifier
            CParserToken &tkn = aTokens[aTokens.size() - 1];
            const Str_t &strName = tkn.GetValue().ToString();

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

};

#endif // (Dreamy Utilities Include Guard)
