//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_PARSERDATA_H
#define _DREAMYUTILITIES_INCL_PARSERDATA_H

#include "../DreamyUtilitiesBase.hpp"

#include "Token.hpp"
#include "../Types/String.hpp"

namespace dreamy {

// Current parser data
class CParserData {

public:
  // String data
  const CString str;

  const c8 *pchCur; // Current character
  const c8 *pchNext; // Next character

  // Parser position
  u32 iLineCur; // Current line (from 0)
  u32 iLineBeg; // Line start position character-wise

  CTokenPos pos; // Token beginning and end positions

public:
  // Default constructor
  CParserData(const CString &strSet);

  // Start from a new character
  void Start(void);

  // Check if parser is at the end of the string
  bool AtEnd(void);

  // Set to the current character
  void SetToCurrent(void);

  // Advance character index in either direction
  void Advance(u32 i);

  // Parse another character, if possible (for tokenizers)
  bool CanParse(void);

  // Count one line
  void CountLine(void);

  // Set line and character position based on a character index
  void SetPosition(u32 iPos);

  // Extract a part of the string within the [begin - current character] range
  CString ExtractString(u32 iBeginOffset);

// Parsing from the current character one time
public:

  // Add end-of-file token at the very end
  void AddEOF(CTokenList &aTokens);

  // Tokenize C/C++ styled comments
  bool ParseComments(CTokenList &aTokens, bool bTokenize);

  // Tokenize operators
  bool ParseOperators(CTokenList &aTokens);

  // Parse a string enclosed within certain characters
  bool ParseString(CString &str, const c8 chEnclosed);

  // Tokenize character sequences
  bool ParseCharSequences(CTokenList &aTokens, const c8 chString, const c8 chCharSeq);

  // Tokenize numbers
  bool ParseNumbers(CTokenList &aTokens);

  // Tokenize words
  bool ParseIdentifiers(CTokenList &aTokens);
};

// General tokenization of a string
void TokenizeString(CTokenList &aTokens, const CString &str, bool bTokenizeComments = false);

};

#endif // (Dreamy Utilities Include Guard)
