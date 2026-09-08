//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#include "CLikeParser.hpp"

namespace dreamy {

bool CCLikeParser::ParseComments(CTokenList &aTokens, bool bTokenize) {
  if (*pchCur != '/') {
    return false;
  }

  switch (*pchNext) {
    // Single-line comment
    case '/': {
      // Skip until the line break
      while (!AtEnd()) {
        SetToCurrent();

        if (*pchCur == '\r' || *pchCur == '\n') {
          break;
        }

        Advance(1);
      }

      // Parse but don't tokenize
      if (!bTokenize) {
        return true;
      }

      // Skip comment opening
      CString strComment = ExtractString(2);
      AddToken(aTokens, CParserToken::TKN_COMMENT, pos, strComment);
    } return true;

    // Multi-line comment
    case '*': {
      // Skip until the comment closing
      while (!AtEnd()) {
        if (str[pos.iLast + 0] == '*'
          && str[pos.iLast + 1] == '/') {
          break;

        // Count lines
        } else if (str[pos.iLast] == '\n') {
          CountLine();
        }

        Advance(1);
      }

      // Parse but don't tokenize
      if (!bTokenize) {
        Advance(2);
        return true;
      }

      // Skip comment opening
      CString strComment = ExtractString(2);

      // Count comment closing
      pos.iLast += 2;

      AddToken(aTokens, CParserToken::TKN_COMMENT, pos, strComment);
    } return true;
  }

  return false;
};

bool CCLikeParser::ParseOperators(CTokenList &aTokens) {
  switch (*pchCur) {
    // Operators
    case '+': {
      switch (*pchNext) {
        case '=': Advance(1); AddToken(aTokens, *pchCur, pos, *pchNext); break; // +=
        case '+': Advance(1); AddToken(aTokens, *pchCur, pos, *pchNext); break; // ++
        default: AddToken(aTokens, *pchCur, pos, 0); // +
      }
    } return true;

    case '-': {
      switch (*pchNext) {
        case '=': Advance(1); AddToken(aTokens, *pchCur, pos, *pchNext); break; // -=
        case '-': Advance(1); AddToken(aTokens, *pchCur, pos, *pchNext); break; // --
        default: AddToken(aTokens, *pchCur, pos, 0); // -
      }
    } return true;

    case '*': {
      if (*pchNext == '=') {
        Advance(1);
        AddToken(aTokens, *pchCur, pos, *pchNext); // *=
      } else {
        AddToken(aTokens, *pchCur, pos, 0); // *
      }
    } return true;

    case '/': {
      if (*pchNext == '=') {
        Advance(1);
        AddToken(aTokens, *pchCur, pos, *pchNext); // /=
      } else {
        AddToken(aTokens, *pchCur, pos, 0); // /
      }
    } return true;

    // Bitwise operators
    case '|': {
      switch (*pchNext) {
        case '=': Advance(1); AddToken(aTokens, *pchCur, pos, *pchNext); break; // |=
        case '|': Advance(1); AddToken(aTokens, *pchCur, pos, *pchNext); break; // ||
        default: AddToken(aTokens, *pchCur, pos, 0); // |
      }
    } return true;

    case '&': {
      switch (*pchNext) {
        case '=': Advance(1); AddToken(aTokens, *pchCur, pos, *pchNext); break; // &=
        case '&': Advance(1); AddToken(aTokens, *pchCur, pos, *pchNext); break; // &&
        default: AddToken(aTokens, *pchCur, pos, 0); // &
      }
    } return true;

    case '^': {
      switch (*pchNext) {
        case '=': Advance(1); AddToken(aTokens, *pchCur, pos, *pchNext); break; // ^=
        case '^': Advance(1); AddToken(aTokens, *pchCur, pos, *pchNext); break; // ^^
        default: AddToken(aTokens, *pchCur, pos, 0); // ^
      }
    } return true;

    // Other operators
    case '>': {
      switch (*pchNext) {
        case '=': Advance(1); AddToken(aTokens, *pchCur, pos, *pchNext); break; // >=
        case '>': Advance(1); AddToken(aTokens, *pchCur, pos, *pchNext); break; // >>
        default: AddToken(aTokens, *pchCur, pos, 0); // >
      }
    } return true;

    case '<': {
      switch (*pchNext) {
        case '=': Advance(1); AddToken(aTokens, *pchCur, pos, *pchNext); break; // <=
        case '<': Advance(1); AddToken(aTokens, *pchCur, pos, *pchNext); break; // <<
        default: AddToken(aTokens, *pchCur, pos, 0); // <
      }
    } return true;

    case '=': {
      if (*pchNext == '=') {
        Advance(1);
        AddToken(aTokens, *pchCur, pos, *pchNext); // ==
      } else {
        AddToken(aTokens, *pchCur, pos, 0); // =
      }
    } return true;

    case '!': {
      if (*pchNext == '=') {
        Advance(1);
        AddToken(aTokens, *pchCur, pos, *pchNext); // !=
      } else {
        AddToken(aTokens, *pchCur, pos, 0); // !
      }
    } return true;

    // Other symbols
    case '#': case '$': case '@': case '`': case '~': case '%': case '?': {
      AddToken(aTokens, *pchCur, pos, 0);
    } return true;
  }

  return false;
};

bool CCLikeParser::ParseString(CString &str, const c8 chEnclosed) {
  if (*pchCur != chEnclosed) {
    return false;
  }

  u8 iEscapeChar = 0; // 1 - unmarked escape character, 2 - marked

  // Go through characters
  while (!AtEnd()) {
    // Current character
    SetToCurrent();

    // Sequence end
    if (*pchCur == chEnclosed) {
      if (!(iEscapeChar & 2)) {
        break;
      }

    // Line break
    } else if (*pchCur == '\n') {
      CountLine();

    // Mark escape characters
    } else if (*pchCur == '\\') {
      iEscapeChar = (iEscapeChar & 2 ? 1 : 2);
      Advance(1);
      continue;
    }

    // Unmark escape character
    if (iEscapeChar & 2) {
      iEscapeChar = 1;
    }

    Advance(1);
  }

  // Didn't parse past the limit
  if (!AtEnd()) {
    // Skip opening character
    str = ExtractString(1);

    // Convert character escape sequences
    if (iEscapeChar != 0) {
      try {
        str.ConvertEscapeChars();

      // Couldn't convert some sequence
      } catch (CFormattingException &ex) {
        SetPosition(pos.iFirst + (u32)ex.GetCharacter());
        throw CTokenException(pos, "Unrecognized character escape sequence");
      }
    }

    // Count closing character
    ++pos.iLast;

  } else {
    throw CTokenException(pos, "Unclosed character sequence");
  }

  return true;
};

bool CCLikeParser::ParseCharSequences(CTokenList &aTokens, const c8 chString, const c8 chCharSeq) {
  CString str;

  // Add string
  if (chString != '\0' && ParseString(str, chString)) {
    AddToken(aTokens, CParserToken::TKN_VALUE, pos, str);
    return true;

  // Add character sequence
  } else if (chCharSeq != '\0' && ParseString(str, chCharSeq)) {
    if (str[0] == '\0') {
      throw CTokenException(pos, "Character sequence cannot be empty");
    } else if (str[1] != '\0') {
      throw CTokenException(pos, "Character sequence is too long");
    }

    AddToken(aTokens, CParserToken::TKN_VALUE, pos, static_cast<s64>(str[0]));
    return true;
  }

  return false;
};

bool CCLikeParser::ParseNumbers(CTokenList &aTokens) {
  if (*pchCur < '0' || *pchCur > '9') {
    return false;
  }

  u8 ubType = 0; // 1 - hex, 2 - real, 3 - scientific notation
  bool bHexCheck = (*pchCur == '0');

  // Parse through numbers
  while (!AtEnd()) {
    SetToCurrent();

    // Change type
    if (*pchCur == 'x' || *pchCur == 'X') {
      // Can't be a hexadecimal number anymore
      if (!bHexCheck) {
        break;
      }

      // Register as a hexadecimal number
      ubType = 1;
      Advance(1);

    // Encountered a dot
    } else if (*pchCur == '.') {
      // Already changed type
      if (ubType != 0) {
        break;
      }

      // Register as a real number
      ubType = 2;
      Advance(1);

    // Encountered scientific notation for a non-hexadecimal
    } else if (ubType != 1 && (*pchCur == 'e' || *pchCur == 'E')) {
      // Already changed type
      if (ubType == 3) {
        break;
      }

      // Not followed by a unary operator or a number
      if (*pchNext != '+' && *pchNext != '-'
        && (*pchNext < '0' || *pchNext > '9')) {
        break;
      }

      // Register as a scientific notation
      ubType = 3;

      // Skip scientific notation with a following character
      Advance(2);

    // Copy other numbers
    } else if (*pchCur >= '0' && *pchCur <= '9') {
      Advance(1);

    // Hexadecimal numbers
    } else if (ubType == 1) { 
      if ((*pchCur >= 'a' && *pchCur <= 'f')
       || (*pchCur >= 'A' && *pchCur <= 'F')) {
        Advance(1);
      } else {
        break;
      }

    // Invalid symbol
    } else {
      break;
    }

    // Stop checking for hexadecimal numbers
    bHexCheck = false;
  }

  // Save the number
  CString strString = ExtractString(0);

  switch (ubType) {
    case 0: { // Integer
      s64 iValue = strString.ToS64();
      AddToken(aTokens, CParserToken::TKN_VALUE, pos, iValue);
    } break;

    case 1: { // Hexadecimal integer
      s64 iHexValue = 0;
      sscanf_s(strString.c_str(), "%llx", &iHexValue);
      AddToken(aTokens, CParserToken::TKN_VALUE, pos, iHexValue);
    } break;

    default: { // Real number
      f64 fValue = atof(strString.c_str());
      AddToken(aTokens, CParserToken::TKN_VALUE, pos, fValue);
    } break;
  }

  return true;
};

bool CCLikeParser::ParseKeys(CTokenList &aTokens) {
  // Start identifier names with an underscore or letters
  if (*pchCur == '_'
  || (*pchCur >= 'a' && *pchCur <= 'z')
  || (*pchCur >= 'A' && *pchCur <= 'Z')) {
    // Parse name symbols
    while (!AtEnd()) {
      SetToCurrent();

      // Allow underscore, letters and numbers
      if (*pchCur == '_'
      || (*pchCur >= '0' && *pchCur <= '9')
      || (*pchCur >= 'a' && *pchCur <= 'z')
      || (*pchCur >= 'A' && *pchCur <= 'Z')) {
        Advance(1);

      // Invalid symbol
      } else {
        break;
      }
    }

    CString strName = ExtractString(0);
    AddToken(aTokens, CParserToken::TKN_KEY, pos, strName);

    return true;
  }

  return false;
};

void CCLikeParser::TokenizeString(CTokenList &aTokens, const CString &str, bool bTokenizeComments) {
  CCLikeParser data(str);

  while (data.CanParse()) {
    switch (*data.GetCurrentChar()) {
      // Skip spaces
      case ' ': case '\t': case '\r': break;

      // Line break
      case '\n': data.CountLine(); break;

      default: {
        // Special tokenizers
        bool bTokenized = data.ParseComments(aTokens, bTokenizeComments)
          || data.ParseKeys(aTokens)
          || data.ParseNumbers(aTokens)
          || data.ParseOperators(aTokens)
          || data.ParseCharSequences(aTokens, '"', '\'');

        if (!bTokenized) {
          // Tokenize every other character
          AddToken(aTokens, (u32)*data.GetCurrentChar(), data.GetTokenPos(), *data.GetCurrentChar());
        }
      } break;
    }
  }

  // String end
  data.AddEOF(aTokens);
};

}; // namespace dreamy
