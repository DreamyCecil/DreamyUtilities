//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_PARSER_TOKENIZER_H
#define _DREAMYUTILITIES_INCL_PARSER_TOKENIZER_H

#include "../Base/Base.hpp"

#include "../Variant/Variant.hpp"
#include "../Parser/ParserData.hpp"
#include "../Parser/Token.hpp"

namespace dreamy
{
  // General tokenization of a string
  inline void TokenizeString(CTokenList &aTokens, const Str_t &str, bool bTokenizeComments = false) {
    CParserData data(str);

    while (data.CanParse()) {
      switch (*data.pchCur) {
        // Skip spaces
        case ' ': case '\t': case '\r': break;

        // Line break
        case '\n': data.CountLine(); break;

        default: {
          // Special tokenizers
          bool bTokenized = data.ParseComments(aTokens, bTokenizeComments)
            || data.ParseIdentifiers(aTokens)
            || data.ParseNumbers(aTokens)
            || data.ParseOperators(aTokens)
            || data.ParseCharSequences(aTokens, '"', '\'');

          if (!bTokenized) {
            // Tokenize every other character
            u32 iCharType = (u32)*data.pchCur;
            AddIntegerToken(aTokens, iCharType, data.pos, *data.pchCur);
          }
        } break;
      }
    }

    // String end
    const u32 iEndPos = (u32)data.str.length();

    data.pos = CTokenPos(iEndPos, iEndPos, 0);
    data.SetPosition(data.pos.iLast);

    AddToken(aTokens, CParserToken::TKN_EOF, data.pos);
  };

};

#endif // (Dreamy Utilities Include Guard)
