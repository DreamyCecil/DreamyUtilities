//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_CLIKEPARSER_H
#define _DREAMYUTILITIES_INCL_CLIKEPARSER_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

#include "GenericParser.hpp"

namespace dreamy {

// C-like parser for parsing C/C++ styled syntax
class CCLikeParser : public CGenericParser {

public:
  // Default constructor
  CCLikeParser(const CString &strSet) : CGenericParser(strSet)
  {
  };

// One-time parsing from the current character
public:

  // Tokenize C/C++ styled comments
  bool ParseComments(CTokenList &aTokens, bool bTokenize);

  // Tokenize C/C++ styled operators
  bool ParseOperators(CTokenList &aTokens);

  // Parse a string enclosed within certain characters
  bool ParseString(CString &str, const c8 chEnclosed);

  // Tokenize C/C++ styled  character sequences
  bool ParseCharSequences(CTokenList &aTokens, const c8 chString, const c8 chCharSeq);

  // Tokenize C/C++ styled numbers
  bool ParseNumbers(CTokenList &aTokens);

  // Tokenize C/C++ styled identifiers and keywords
  bool ParseKeys(CTokenList &aTokens);

public:

  // General tokenization of a string
  static void TokenizeString(CTokenList &aTokens, const CString &str, bool bTokenizeComments = false);
};

}; // namespace dreamy

#endif // (Dreamy Utilities Include Guard)
