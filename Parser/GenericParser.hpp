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

#ifndef _DREAMYUTILITIES_INCL_GENERICPARSER_H
#define _DREAMYUTILITIES_INCL_GENERICPARSER_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

#include "Token.hpp"
#include "../Types/String.hpp"

NAMESPACE_DREAMY_OPEN

// Generic parser fit for any purpose when parsing manually
class CGenericParser {

protected:
  // String data
  const CString m_str;

  const c8 *m_pchCur; // Current character
  const c8 *m_pchNext; // Next character

  // Parser position
  u32 m_iLineCur; // Current line starting from 1 (with 0 being invalid)
  u32 m_iLineBeg; // Line start position character-wise

  CTokenPos m_pos; // Token beginning and end positions

public:
  // Default constructor
  CGenericParser(const CString &strSet);

  // Get current character
  inline c8 Cur(void) const {
    return *m_pchCur;
  };

  // Get the next character, ahead of the current one
  inline c8 Next(void) const {
    return *m_pchNext;
  };

  // Get the current token position
  inline const CTokenPos &GetTokenPos(void) const {
    return m_pos;
  };

// Token parsing cycle
public:

  // Begin parsing the first token from the beginning of the string
  void Initialize(void);

  // Check if the parser has reached the end of the string
  bool AtEnd(void);

  // Begin parsing a new token from the current position
  void StartToken(void);

  // Convenience macro for parsing until the end
  // After manually tokenizing a part of the string, the parsed token must be skipped by advancing the
  // current character index in order to begin parsing a new token on the next loop cycle, otherwise it
  // may lead to an infinite loop by trying to tokenize the exact same part of the string over and over
  #define DREAMY_PARSE_FOR(_Parser) \
    for ((_Parser).Initialize(); !(_Parser).AtEnd(); (_Parser).StartToken())

protected:
  // Update the current character based on the current position
  void SetCurrentChar(void);

public:
  // Rewind the character index to the beginning of the token
  void Rewind(void);

  // Set character index to a specific position relative to the beginning of the token
  void SetPos(u32 iFromTokenStart);

  // Advance character index in either direction
  void Advance(s32 iOffset);

  // Count one line and set new line beginning to the current token end
  void CountLine(void);

  // Set line and column based on absolute character position
  void UpdateTokenPlace(u32 iPos);

  // Get current token length (i.e. amount of parsed characters since the token start)
  inline u32 GetTokenLength(void) const {
    return m_pos.Length();
  };

  // Extract a part of the string within the [token start + offset .. current character] range
  CString ExtractString(u32 iBeginOffset = 0);

  // Add end-of-file token at the very end
  void AddEOF(CTokenList &aTokens);
};

NAMESPACE_DREAMY_CLOSE

#endif // (Dreamy Utilities Include Guard)
