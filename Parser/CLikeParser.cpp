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

#include "CLikeParser.hpp"

namespace dreamy {

bool CCLikeParser::ParseComment(CString *pstr)
{
  if (Cur() != '/') return false;

  // Single-line comment
  if (Next() == '/') {
    // Skip until a line break
    do {
      Advance(1);
      if (Cur() == '\r' || Cur() == '\n') break;
    } while (!AtEnd());

    // Skip comment opening
    if (pstr != nullptr) *pstr = ExtractString(2);

    return true;

  // Multi-line comment
  } else if (Next() == '*') {
    // Skip until the comment closing
    do {
      Advance(1);
      if (Cur() == '*' && Next() == '/') break;

      // Count lines
      if (Cur() == '\n') CountLine();
    } while (!AtEnd());

    // Skip comment opening
    if (pstr != nullptr) *pstr = ExtractString(2);

    // Comment closing
    Advance(2);

    return true;
  }

  return false;
};

bool CCLikeParser::TokenizeComment(CTokenList *paTokens) {
  if (paTokens == nullptr) return ParseComment();

  CString str;
  if (!ParseComment(&str)) return false;
  AddToken(*paTokens, CParserToken::TKN_COMMENT, m_pos, str);
  return true;
};

bool CCLikeParser::ParseOperator(CString *pstr) {
  const c8 chType = Cur();
  const c8 chNext = Next();
  c8 chSubType = '\0';

  switch (chType) {
    // Arithmetic operators
    case '+': {
      // +=  ++
      if (chNext == '=' || chNext == '+') chSubType = chNext;
    } break;

    case '-': {
      // -=  --
      if (chNext == '=' || chNext == '-') chSubType = chNext;
    } break;

    case '*': {
      // *=
      if (chNext == '=') chSubType = chNext;
    } break;

    case '/': {
      // /=
      if (chNext == '=') chSubType = chNext;
    } break;

    // Bitwise operators
    case '|': {
      // |=  ||
      if (chNext == '=' || chNext == '|') chSubType = chNext;
    } break;

    case '&': {
      // &=  &&
      if (chNext == '=' || chNext == '&') chSubType = chNext;
    } break;

    case '^': {
      // ^=  ^^
      if (chNext == '=' || chNext == '^') chSubType = chNext;
    } break;

    // Other operators
    case '>': {
      // >=  >>
      if (chNext == '=' || chNext == '>') chSubType = chNext;
    } break;

    case '<': {
      // <=  <<
      if (chNext == '=' || chNext == '<') chSubType = chNext;
    } break;

    case '=': {
      // ==
      if (chNext == '=') chSubType = chNext;
    } break;

    case '!': {
      // !=
      if (chNext == '=') chSubType = chNext;
    } break;

    // Other symbols
    case '#': case '$': case '@': case '`': case '~': case '%': case '?':
      break;

    // Invalid symbol
    default: return false;
  }

  if (pstr != nullptr) {
    CString &str = *pstr;
    str = "  ";
    str[0] = chType;
    str[1] = chSubType;
  }

  Advance((chSubType != '\0') ? 2 : 1);
  return true;
};

bool CCLikeParser::TokenizeOperator(CTokenList *paTokens) {
  if (paTokens == nullptr) return ParseOperator();

  CString str;
  if (!ParseOperator(&str)) return false;

  // Returned operator string should be at least one character long and the subtype
  // can either be the second character or 0 for none (from the null terminator)
  D_ASSERT(str[0] != '\0');
  AddToken(*paTokens, static_cast<u8>(str[0]), m_pos, static_cast<u8>(str[1]));
  return true;
};

bool CCLikeParser::ParseString(const c8 chEnclosed, CString *pstr)
{
  if (Cur() != chEnclosed) return false;

  bool bHasEscSeq = false; // Whether the string contains any escape sequences
  bool bEscSeq = false; // Whether parsing an escape sequence

  do {
    // Get next character (or first one after the opening character)
    Advance(1);

    // Sequence end
    if (Cur() == chEnclosed) {
      if (!bEscSeq) break;

    // Line break
    } else if (Cur() == '\n') {
      // If not escaped, the string was not closed
      if (!bEscSeq) {
        throw CTokenException(m_pos, "Unclosed character sequence");
      }

      CountLine();

    // Carriage return
    } else if (Cur() == '\r') {
      // If escaping, let the line break go through on the next cycle
      if (bEscSeq && Next() == '\n') {
        continue;
      }

    // Parse escape sequences after a backslash
    } else if (Cur() == '\\') {
      bEscSeq = !bEscSeq;
      bHasEscSeq = true;
      continue;
    }

    // Parsed an escape sequence
    bEscSeq = false;
  } while (!AtEnd());

  // Parsed past the limit
  if (AtEnd()) {
    throw CTokenException(m_pos, "Unclosed character sequence");
  }

  if (pstr != nullptr) {
    // Skip opening character
    *pstr = ExtractString(1);

    // Convert character escape sequences
    if (bHasEscSeq) {
      try {
        pstr->ConvertEscapeChars();

      // Couldn't convert some sequence
      } catch (CFormattingException &ex) {
        UpdateTokenPlace(m_pos.GetStart() + (u32)ex.GetPos());
        throw CTokenException(m_pos, "Unrecognized character escape sequence");
      }
    }
  }

  // Closing character
  Advance(1);

  return true;
};

bool CCLikeParser::TokenizeCharSequence(const c8 chString, const c8 chLiteral, CTokenList *paTokens) {
  CString str;

  // Add string
  if (chString != '\0' && ParseString(chString, &str)) {
    if (paTokens != nullptr) {
      AddToken(*paTokens, CParserToken::TKN_VALUE, m_pos, str);
    }
    return true;

  // Add character sequence
  } else if (chLiteral != '\0' && ParseString(chLiteral, &str)) {
    if (str[0] == '\0') {
      throw CTokenException(m_pos, "Character sequence cannot be empty");
    } else if (str[1] != '\0') {
      throw CTokenException(m_pos, "Character sequence is too long");
    }

    if (paTokens != nullptr) {
      AddToken(*paTokens, CParserToken::TKN_VALUE, m_pos, static_cast<u8>(str[0]));
    }
    return true;
  }

  return false;
};

bool CCLikeParser::ParseKey(CString *pstr) {
  // Start identifier names with an underscore or letters
  if (Cur() == '_'
  || (Cur() >= 'a' && Cur() <= 'z')
  || (Cur() >= 'A' && Cur() <= 'Z')) {
    // Parse name symbols until any invalid character
    do {
      Advance(1);

      // Allow underscore, letters and numbers
      if (Cur() != '_'
      && (Cur() < '0' || Cur() > '9')
      && (Cur() < 'a' || Cur() > 'z')
      && (Cur() < 'A' || Cur() > 'Z')) {
        break;
      }
    } while (!AtEnd());

    if (pstr != nullptr) *pstr = ExtractString();
    return true;
  }

  return false;
};

bool CCLikeParser::TokenizeKey(CTokenList *paTokens) {
  if (paTokens == nullptr) return ParseKey();

  CString str;
  if (!ParseKey(&str)) return false;
  AddToken(*paTokens, CParserToken::TKN_KEY, m_pos, str);
  return true;
};

u32 CCLikeParser::ParseHexInteger(s64 *piValue, bool bHexPrefix) {
  if (bHexPrefix) {
    // Must start with "0x"
    if (Cur() != '0' || (Next() != 'x' && Next() != 'X')) return 0;
    Advance(2);

  } else {
    // Must start with at least one digit
    u8 ch = Cur();
    if ((ch < '0' || ch > '9') && (ch < 'a' || ch > 'f') && (ch < 'A' || ch > 'F')) return 0;
  }

  const u32 iTokenOffset = GetTokenLength(); // Position of the first real digit
  u64 iValue = 0;

  // Go through hexadecimal digits until there are no more
  for (; !AtEnd(); Advance(1)) {
    u8 ch = Cur();

    if (ch >= '0' && ch <= '9') {
      iValue = (iValue << 4) + (ch - '0');
    } else if (ch >= 'a' && ch <= 'f') {
      iValue = (iValue << 4) + (ch - 'a' + 10);
    } else if (ch >= 'A' && ch <= 'F') {
      iValue = (iValue << 4) + (ch - 'A' + 10);
    } else {
      break;
    }
  }

  // Save the integer value
  if (piValue != nullptr) *piValue = static_cast<s64>(iValue);

  return GetTokenLength() - iTokenOffset;
};

u32 CCLikeParser::ParseOctInteger(s64 *piValue, bool bOctPrefix) {
  if (bOctPrefix) {
    // Must start with 0 and at least one octal digit (don't advance here like with "0x")
    //if (Cur() != '0' || (Next() < '0' || Next() > '7')) return 0;

    // Must start with "0o"
    if (Cur() != '0' || (Next() != 'o' && Next() != 'O')) return 0;
    Advance(2);

  } else {
    // Must start with at least one digit
    if (Cur() < '0' || Cur() > '7') return 0;
  }

  const u32 iTokenOffset = GetTokenLength(); // Position of the first real digit
  u64 iValue = 0;

  // Go through octal digits until there are no more
  for (; !AtEnd(); Advance(1)) {
    u8 ch = Cur();

    if (ch >= '0' && ch <= '7') {
      iValue = (iValue << 3) + (ch - '0');
    } else {
      break;
    }
  }

  // Save the integer value
  if (piValue != nullptr) *piValue = static_cast<s64>(iValue);

  return GetTokenLength() - iTokenOffset;
};

u32 CCLikeParser::ParseBinInteger(s64 *piValue, bool bBinPrefix) {
  if (bBinPrefix) {
    // Must start with "0b"
    if (Cur() != '0' || (Next() != 'b' && Next() != 'B')) return 0;
    Advance(2);

  } else {
    // Must start with at least one digit
    if (Cur() != '0' && Cur() != '1') return 0;
  }

  const u32 iTokenOffset = GetTokenLength(); // Position of the first real digit
  u64 iValue = 0;

  // Go through binary digits until there are no more
  for (; !AtEnd(); Advance(1)) {
    u8 ch = Cur();

    if (ch == '0') {
      iValue <<= 1;
    } else if (ch == '1') {
      iValue = (iValue << 1) | 1;
    } else {
      break;
    }
  }

  // Save the integer value
  if (piValue != nullptr) *piValue = static_cast<s64>(iValue);

  return GetTokenLength() - iTokenOffset;
};

u32 CCLikeParser::ParseDecInteger(s64 *piValue) {
  // Must start with at least one digit
  if (Cur() < '0' || Cur() > '9') return 0;

  const u32 iTokenOffset = GetTokenLength(); // Position of the first real digit
  u64 iValue = 0;

  // Go through digits until there are no more
  for (; !AtEnd(); Advance(1)) {
    u8 ch = Cur();

    if (ch >= '0' && ch <= '9') {
      iValue = (iValue * 10) + (ch - '0');
    } else {
      break;
    }
  }

  // Save the integer value
  if (piValue != nullptr) *piValue = static_cast<s64>(iValue);

  return GetTokenLength() - iTokenOffset;
};

bool CCLikeParser::ParseHexFloat(f64 *pfValue, s64 *piInteger) {
  // Must start with "0x"
  if (Cur() != '0' || (Next() != 'x' && Next() != 'X')) return 0;
  Advance(2);

  bool bFloat = false;
  s64 iWhole = 0;
  s64 iFraction = 0;
  u32 iFracCount = 0;
  s64 iExponent = 0;

  // Try parsing the part before a decimal point
  bool bPreDot = (ParseHexInteger(&iWhole, false) != 0);

  // Then the part after the decimal point
  if (Cur() == '.') {
    bFloat = true;
    Advance(1);

    iFracCount = ParseHexInteger(&iFraction, false);

    // Should have at least one of the decimal sides
    if (!bPreDot && iFracCount == 0) {
      Rewind();
      return false;
    }
  }

  // Finally, try the exponent
  if (Cur() == 'p' || Cur() == 'P') {
    bFloat = true;
    Advance(1);

    // Optional sign
    bool bNegative = false;
    c8 chSign = Cur();

    if (chSign == '+' || chSign == '-') {
      bNegative = (chSign == '-');
      Advance(1);
    }

    // Parse required exponent (must still be decimal)
    if (!ParseDecInteger(&iExponent)) {
      Rewind();
      return false;
    }

    // Negate the exponent
    if (bNegative) iExponent = -iExponent;
  }

  // Save the integer value
  if (piInteger != nullptr) {
    *piInteger = (bFloat ? 0 : iWhole);
  }

  // Assemble and save the float value
  if (pfValue != nullptr) {
    f64 fFinal = static_cast<f64>(iWhole);

    if (iFracCount != 0) {
      fFinal += static_cast<f64>(iFraction) * pow(16.0, -static_cast<f64>(iFracCount));
    }

    if (iExponent != 0) {
      fFinal *= pow(2.0, static_cast<f64>(iExponent));
    }

    *pfValue = fFinal;
  }

  return true;
};

bool CCLikeParser::ParseHexIntegerOrFloat(CVariant *pval) {
  // Parse the float without returning it
  if (pval == nullptr) return ParseHexFloat();

  // Parse any valid float value and then distinguish it from an integer
  f64 fValue;
  s64 iValue;
  if (!ParseHexFloat(&fValue, &iValue)) return false;

  // Set integer if it's non-zero or both of them are zero
  if (iValue != 0 || fValue == 0.0) {
    pval->FromInt(iValue);

  // Otherwise set float if it's non-zero and integer is zero
  } else {
    pval->FromFloat(fValue);
  }

  return true;
};

bool CCLikeParser::ParseDecFloat(f64 *pfValue, s64 *piInteger) {
  // Must start with at least one digit or a decimal point
  if (Cur() != '.' && (Cur() < '0' || Cur() > '9')) return false;

  bool bFloat = false;
  s64 iWhole = 0;
  s64 iFraction = 0;
  u32 iFracCount = 0;
  s64 iExponent = 0;

  // Try parsing the part before a decimal point
  bool bPreDot = (ParseDecInteger(&iWhole) != 0);

  // Then the part after the decimal point
  if (Cur() == '.') {
    bFloat = true;
    Advance(1);

    iFracCount = ParseDecInteger(&iFraction);

    // Should have at least one of the decimal sides
    if (!bPreDot && iFracCount == 0) {
      Rewind();
      return false;
    }
  }

  // Finally, try the exponent
  if (Cur() == 'e' || Cur() == 'E') {
    bFloat = true;
    Advance(1);

    // Optional sign
    bool bNegative = false;
    c8 chSign = Cur();

    if (chSign == '+' || chSign == '-') {
      bNegative = (chSign == '-');
      Advance(1);
    }

    // Parse required exponent
    if (!ParseDecInteger(&iExponent)) {
      Rewind();
      return false;
    }

    // Negate the exponent
    if (bNegative) iExponent = -iExponent;
  }

  // Save the integer value
  if (piInteger != nullptr) {
    *piInteger = (bFloat ? 0 : iWhole);
  }

  // Assemble and save the float value
  if (pfValue != nullptr) {
    f64 fFinal = static_cast<f64>(iWhole);

    if (iFracCount != 0) {
      fFinal += static_cast<f64>(iFraction) * pow(10.0, -static_cast<f64>(iFracCount));
    }

    if (iExponent != 0) {
      fFinal *= pow(10.0, static_cast<f64>(iExponent));
    }

    *pfValue = fFinal;
  }

  return true;
};

bool CCLikeParser::ParseDecIntegerOrFloat(CVariant *pval) {
  // Parse the float without returning it
  if (pval == nullptr) return ParseDecFloat();

  // Parse any valid float value and then distinguish it from an integer
  f64 fValue;
  s64 iValue;
  if (!ParseDecFloat(&fValue, &iValue)) return false;

  // Set integer if it's non-zero or both of them are zero
  if (iValue != 0 || fValue == 0.0) {
    pval->FromInt(iValue);

  // Otherwise set float if it's non-zero and integer is zero
  } else {
    pval->FromFloat(fValue);
  }

  return true;
};

bool CCLikeParser::TokenizeNumber(CTokenList *paTokens) {
  if (paTokens == nullptr) {
    // Non-decimal numbers first due to special prefixes, then decimal ones
    return (ParseOctInteger() || ParseBinInteger()
      || ParseHexIntegerOrFloat() || ParseDecIntegerOrFloat());
  }

  s64 iValue;

  // Non-hexadecimal & non-decimal integers first
  if (ParseOctInteger(&iValue) || ParseBinInteger(&iValue)) {
    AddToken(*paTokens, CParserToken::TKN_VALUE, m_pos, iValue);
    return true;

  // Then the rest of the integers + floats
  } else {
    CVariant val;

    // Non-decimal first due to special prefixes
    if (ParseHexIntegerOrFloat(&val) || ParseDecIntegerOrFloat(&val)) {
      AddToken(*paTokens, CParserToken::TKN_VALUE, m_pos, val);
      return true;
    }
  }

  return false;
};

void CCLikeParser::Tokenize(CTokenList &aTokens, const CString &str, bool bTokenizeComments) {
  CCLikeParser data(str);

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

      default: {
        // Special tokenizers
        bool bTokenized = data.TokenizeComment(bTokenizeComments ? &aTokens : nullptr)
          || data.TokenizeKey(&aTokens)
          || data.TokenizeNumber(&aTokens)
          || data.TokenizeOperator(&aTokens)
          || data.TokenizeCharSequence('\"', '\'', &aTokens);

        if (!bTokenized) {
          // Tokenize every other character
          u8 ch = static_cast<u8>(data.Cur());
          AddToken(aTokens, ch, data.GetTokenPos(), ch);
          data.Advance(1);
        }
      } break;
    }
  }

  // String end
  data.AddEOF(aTokens);
};

}; // namespace dreamy
