//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_TOKEN_H
#define _DREAMYUTILITIES_INCL_TOKEN_H

#include "../DreamyUtilitiesBase.hpp"

#include "../Math/Algorithm.hpp"
#include "../Types/Exception.hpp"
#include "../Types/Variant.hpp"

namespace dreamy {

// Token position within the string
class CTokenPos {

public:
  u32 iFirst; // First character index
  u32 iLast; // Last character index

  u32 iLine; // Line index (from 0)
  u32 iCol; // Column index (from 0)

public:
  // Default constructor
  CTokenPos() : iFirst(-1), iLast(-1), iLine(-1), iCol(-1)
  {
  };

  // Constructor with positions
  CTokenPos(u32 iSetFirst, u32 iSetLast, u32 iSetLine, u32 iSetCol) :
    iFirst(iSetFirst), iLast(iSetLast), iLine(iSetLine), iCol(iSetCol)
  {
  };

  // Copy constructor
  CTokenPos(const CTokenPos &posOther) {
    operator=(posOther);
  };

  // Get token length
  inline u32 Length(void) const {
    return iLast - iFirst;
  };

  // Shift token position (doesn't affect formatted position)
  inline void ShiftPos(s32 iOffset) {
    iFirst += iOffset;
    iLast += iOffset;
  };

  // Format token position relative to the line
  inline void FormatPos(u32 iCurrentPos, u32 iCurrentLine, u32 iLineBeginning) {
    iLine = iCurrentLine;
    iCol = iCurrentPos - iLineBeginning;
  };

  // Assignment
  inline CTokenPos &operator=(const CTokenPos &posOther) {
    iFirst = posOther.iFirst;
    iLast  = posOther.iLast;
    iLine  = posOther.iLine;
    iCol   = posOther.iCol;

    return *this;
  };

  // Compare positions
  inline bool operator==(const CTokenPos &posOther) const {
    return (iFirst == posOther.iFirst
         && iLast  == posOther.iLast
         && iLine  == posOther.iLine
         && iCol   == posOther.iCol);
  };

  // Retrieve token line and column
  inline void GetPos(u32 &iGetLine, u32 &iGetCol) const {
    // Start from 1
    iGetLine = iLine + 1;
    iGetCol = iCol + 1;
  };
};

// Exception that a tokenizer may throw
class CTokenException : public CMessageException {

protected:
  CTokenPos _pos;

public:
  // Default constructor
  CTokenException(const CTokenPos &pos, const c8 *strError = "") : _pos(pos)
  {
    u32 iLine, iCol;
    _pos.GetPos(iLine, iCol);

    PrintF("%s at line %u, col %u", strError, iLine, iCol);
  };

  // Get error position
  const CTokenPos GetPos(void) const {
    return _pos;
  };

  // Quick function for throwing token exceptions
  static void Throw(const CTokenPos &pos, const c8 *strFormat, ...) {
    CTokenException ex(pos);
    DREAMY_PRINTF_INLINE(ex._message, strFormat);

    throw ex;
  };
};

// One token from a string
class CParserToken {

public:
  // Token type
  enum EType {
    TKN_INVALID = 0xFFFFFFFF,

    // Individual tokens (from 0 to 31)
    TKN_COMMENT = 0x00, // Information

    TKN_IDENTIFIER = 0x01, // Unique word
    TKN_VALUE      = 0x02, // Any value

    TKN_KEYWORD    = 0x03, // Action word
    TKN_OPERATOR   = 0x04, // Punctuation action
    TKN_ASSIGNMENT = 0x05, // Assignment operation

    TKN_NEWLINE = 0x0A, // Newline character (same as '\n')

    // Pairs
    TKN_BLOCK_OPEN  = '{',
    TKN_BLOCK_CLOSE = '}',
    TKN_GROUP_OPEN  = '[',
    TKN_GROUP_CLOSE = ']',
    TKN_PAR_OPEN    = '(',
    TKN_PAR_CLOSE   = ')',

    // Punctuation
    TKN_EQUAL   = '=', // Assignment or comparison
    TKN_PERCENT = '%',
    TKN_LEFT  = '<', // To the left
    TKN_RIGHT = '>', // To the right

    TKN_EXCLAMATION = '!', // Negation or inversion
    TKN_QUESTION    = '?', // Condition

    TKN_PERIOD = '.', // Relation
    TKN_COMMA  = ',', // Advancement

    TKN_COLON   = ':', // Relation
    TKN_SEMICOL = ';', // End

    // Arithmetics
    TKN_ADD = '+',
    TKN_SUB = '-',
    TKN_MUL = '*',
    TKN_DIV = '/',

    // Bitwise operators
    TKN_OR  = '|', // "either"
    TKN_AND = '&', // "and or within"
    TKN_XOR = '^', // "both or neither"

    // Unusual
    TKN_HASH   = '#',
    TKN_DOLLAR = '$',
    TKN_AT     = '@',
    TKN_UNDER  = '_', // Within the identifier name
    TKN_GRAVE  = '`',
    TKN_TILDE  = '~', // Bitwise inversion

    // Special cases
    TKN_QUOTE       = '"',  // Usually enclosing strings
    TKN_SINGLEQUOTE = '\'', // Usually enclosing characters
    TKN_BACKSLASH   = '\\', // Usually within context (e.g. escape characters)

    // End of file/data
    TKN_END = 256,

    // Last token for offsetting (new tokens can be verified by ignoring the first 9 bits)
    TKN_LASTTOKEN = 512,
  };

protected:
  u32 _type;      // Token type
  CTokenPos _pos; // Token position
  CVariant _val;  // Token value

  // Make sure that it's an appropriate token type
  inline void AssertType(u32 iCheckType) const {
    if (iCheckType != _type) {
      CTokenException::Throw(GetTokenPos(), "Expected token under ID '%d' but got '%d'", iCheckType, _type);
    }
  };

public:
  // Default constructor
  CParserToken(u32 iSetType = TKN_INVALID) :
    _type(iSetType), _pos(), _val()
  {
  };

  // Constructor with a value
  CParserToken(u32 iSetType, CTokenPos posSet, const CVariant &valSet)
    : _type(iSetType), _pos(posSet), _val(valSet)
  {
  };

  // Get token type
  inline u32 GetType(void) const {
    return _type;
  };

  // Get token position
  inline CTokenPos &TokenPos(void) {
    return _pos;
  };

  // Get token position (read-only)
  inline const CTokenPos &GetTokenPos(void) const {
    return _pos;
  };

  // Get token value
  inline CVariant &Value(void) {
    return _val;
  };

  // Get token value (read-only)
  inline const CVariant &GetValue(void) const {
    return _val;
  };

  // Get position string
  const CString PrintPos(void) const {
    u32 iLine, iCol;
    GetTokenPos().GetPos(iLine, iCol);

    // Print a line and a column
    CString str;
    str.PrintF("line %u, col %u", iLine, iCol);

    return str;
  };

  // Only return this token if it matches the desired type
  const CParserToken &operator()(u32 iCheckType) const {
    AssertType(iCheckType);
    return *this;
  };

  // Compare tokens
  bool operator==(const CParserToken &tokenOther) const {
    // Compare types if the position is invalid
    if (tokenOther.GetTokenPos() == CTokenPos()) {
      return GetType() == tokenOther.GetType();
    }

    // Compare token data
    return (GetType() == tokenOther.GetType())
        && (GetTokenPos() == tokenOther.GetTokenPos())
        && (GetValue() == tokenOther.GetValue());
  };
};

// List of tokens
typedef std::vector<CParserToken> CTokenList;

// Add one parser token
inline void AddToken(CTokenList &aTokens, u32 iType,
                      const CTokenPos &pos, const CVariant &val = CVariant()) {
  aTokens.push_back(CParserToken(iType, pos, val));
};

}; // namespace dreamy

#endif // (Dreamy Utilities Include Guard)
