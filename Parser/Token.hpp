//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_TOKEN_H
#define _DREAMYUTILITIES_INCL_TOKEN_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

#include "../Math/Algorithm.hpp"
#include "../Types/Exception.hpp"
#include "../Types/Variant.hpp"

NAMESPACE_DREAMY_OPEN

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
  CTokenPos m_pos;

public:
  // Default constructor
  CTokenException(const CTokenPos &pos, const c8 *strError = "") : m_pos(pos)
  {
    u32 iLine, iCol;
    m_pos.GetPos(iLine, iCol);

    PrintF("%s at line %u, col %u", strError, iLine, iCol);
  };

  // Get error position
  const CTokenPos GetPos(void) const {
    return m_pos;
  };

  // Quick function for throwing token exceptions
  static void Throw(const CTokenPos &pos, const c8 *strFormat, ...) {
    CTokenException ex(pos);
    DREAMY_PRINTF_INLINE(ex.m_strMessage, strFormat);

    throw ex;
  };
};

// One token from a string
class CParserToken {

public:
  // Token type
  enum EType {
    TKN_INVALID = 0xFFFFFFFF, // Invalid token
    TKN_DEFAULT_FIRST = 0, // First default token

    TKN_COMMENT = 0x00, // Information (NUL)
    TKN_KEY     = 0x01, // Identifier or keyword (SOH)
    TKN_VALUE   = 0x02, // Any value (STX)
    // Reserved = 0x03
    TKN_EOF     = 0x04, // End of file/data (EOT)
    // Reserved = 0x05 .. 0x09
    TKN_NEWLINE = '\n', // End of line (LF)
    // Reserved = 0x0B .. 0xFF

    TKN_DEFAULT_LAST = TKN_NEWLINE, // Last default token
    TKN_DEFAULT_MASK = 0xFF, // Bit mask that fits all of the default token types

    TKN_CUSTOM_FIRST = TKN_DEFAULT_MASK + 1, // First custom token
  };

protected:
  u32 m_iType;      // Token type
  CTokenPos m_pos; // Token position
  CVariant m_val;  // Token value

  // Make sure that it's an appropriate token type
  inline void AssertType(u32 iCheckType) const {
    if (iCheckType == m_iType) return;

    bool bPrintableCheck = (iCheckType > 0x20 && iCheckType < 0x7F);
    bool bPrintableType = (m_iType > 0x20 && m_iType < 0x7F);

    const c8 *strError;

    if (bPrintableCheck && bPrintableType) {
      strError = "Expected token under ID '%c' but got '%c'";
    } else if (bPrintableCheck) {
      strError = "Expected token under ID '%c' but got %u";
    } else if (bPrintableType) {
      strError = "Expected token under ID %u but got '%c'";
    } else {
      strError = "Expected token under ID %u but got %u";
    }

    CTokenException::Throw(GetTokenPos(), strError, iCheckType, m_iType);
  };

public:
  // Default constructor
  CParserToken(u32 iSetType = TKN_INVALID) :
    m_iType(iSetType), m_pos(), m_val()
  {
  };

  // Constructor with a value
  CParserToken(u32 iSetType, CTokenPos posSet, const CVariant &valSet)
    : m_iType(iSetType), m_pos(posSet), m_val(valSet)
  {
  };

  // Get token type
  inline u32 GetType(void) const {
    return m_iType;
  };

  // Get token position
  inline CTokenPos &GetTokenPos(void) {
    return m_pos;
  };

  // Get token position (read-only)
  inline const CTokenPos &GetTokenPos(void) const {
    return m_pos;
  };

  // Get token value
  inline CVariant &GetValue(void) {
    return m_val;
  };

  // Get token value (read-only)
  inline const CVariant &GetValue(void) const {
    return m_val;
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
  inline const CParserToken &Verify(u32 iCheckType) const {
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
inline void AddToken(CTokenList &aTokens, u32 iType, const CTokenPos &pos, const CVariant &val = CVariant()) {
  aTokens.push_back(CParserToken(iType, pos, val));
};

NAMESPACE_DREAMY_CLOSE

#endif // (Dreamy Utilities Include Guard)
