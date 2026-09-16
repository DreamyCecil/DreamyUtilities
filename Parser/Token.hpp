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

private:
  // Token "range" from the starting character until the ending character, i.e. the last character
  // of the token itself is "iEnd - 1" with "iEnd" being the first character right after the token
  u32 iStart, iEnd;

  // Token "place" in text as line and column starting from 1 (with 0 being invalid)
  u32 iLine, iCol;

  friend class CGenericParser;

public:
  // Default constructor with invalid state
  CTokenPos() : iStart(0), iEnd(0), iLine(0), iCol(0)
  {
  };

  // Constructor with range and place
  CTokenPos(u32 iSetStart, u32 iSetEnd, u32 iSetLine, u32 iSetCol) :
    iStart(iSetStart), iEnd(iSetEnd), iLine(iSetLine), iCol(iSetCol)
  {
  };

  // Copy constructor
  CTokenPos(const CTokenPos &posOther) {
    operator=(posOther);
  };

  // Get starting token character
  inline u32 GetStart(void) const {
    return iStart;
  };

  // Get ending token character
  inline u32 GetEnd(void) const {
    return iEnd;
  };

  // Get token line
  inline u32 GetLine(void) const {
    return iLine;
  };

  // Get token column
  inline u32 GetCol(void) const {
    return iCol;
  };

  // Get token length
  inline u32 Length(void) const {
    return iEnd - iStart;
  };

  // Check if this position is invalid
  inline bool IsInvalid(void) const {
    return (iLine == 0 || iCol == 0);
  };

  // Set this position to invalid state with optional zero-length token position
  inline void SetInvalid(u32 iPos = 0) {
    iStart = iEnd = iPos;
    iLine = iCol = 0;
  };

  // Shift token range, leaving the line and the column where they are
  inline void ShiftRange(s32 iOffset) {
    iStart += iOffset;
    iEnd += iOffset;
  };

  // Set token line and column
  inline void SetPlace(u32 iSetLine, u32 iSetCol) {
    iLine = iSetLine;
    iCol = iSetCol;
  };

  // Assignment
  inline CTokenPos &operator=(const CTokenPos &posOther) {
    iStart = posOther.iStart;
    iEnd   = posOther.iEnd;
    iLine  = posOther.iLine;
    iCol   = posOther.iCol;
    return *this;
  };

  // Check for equality
  inline bool operator==(const CTokenPos &posOther) const {
    return (iStart == posOther.iStart
         && iEnd   == posOther.iEnd
         && iLine  == posOther.iLine
         && iCol   == posOther.iCol);
  };

  // Check for inequality
  __forceinline bool operator!=(const CTokenPos &posOther) const {
    return !operator==(posOther);
  };
};

// Exception that a tokenizer may throw
class CTokenException : public CMessageException {

protected:
  u32 m_iLine, m_iCol;

public:
  // Default constructor
  CTokenException(const CTokenPos &pos, const c8 *strError) : m_iLine(pos.GetLine()), m_iCol(pos.GetCol())
  {
    PrintF("%s at line %u, col %u", strError, m_iLine, m_iCol);
  };

  // Get error line
  inline u32 GetLine(void) const {
    return m_iLine;
  };

  // Get error column
  inline u32 GetCol(void) const {
    return m_iCol;
  };

  // Quick function for throwing token exceptions
  static void Throw(const CTokenPos &pos, const c8 *strFormat, ...) {
    CString strError;
    DREAMY_PRINTF_INLINE(strError, strFormat);

    throw CTokenException(pos, strError.c_str());
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
    // Print a line and a column
    CString str;
    str.PrintF("line %u, col %u", GetTokenPos().GetLine(), GetTokenPos().GetCol());
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
    if (tokenOther.GetTokenPos().IsInvalid()) {
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
