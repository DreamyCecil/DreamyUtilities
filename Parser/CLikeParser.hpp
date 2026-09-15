//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_CLIKEPARSER_H
#define _DREAMYUTILITIES_INCL_CLIKEPARSER_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

#include "GenericParser.hpp"

NAMESPACE_DREAMY_OPEN

// C-like parser for parsing C/C++ styled syntax
class CCLikeParser : public CGenericParser {

public:
  // Default constructor
  CCLikeParser(const CString &strSet) : CGenericParser(strSet)
  {
  };

// One-time parsing from the current character
// Methods return a non-zero value if they parsed the respective token and moved the current character past it
public:

  // Parse a single-line or a multi-line comment
  // If string is nullptr, parses the comment without returning it
  bool ParseComment(CString *pstr = nullptr);

  // Parse and tokenize a comment some common way
  // If token list is nullptr, parses the comment without tokenizing it
  bool TokenizeComment(CTokenList *paTokens);

  // Parse an expression operator
  // If string is nullptr, parses the operator without returning it
  bool ParseOperator(CString *pstr = nullptr);

  // Parse and tokenize an operator some common way
  // If token list is nullptr, parses the operator without tokenizing it
  bool TokenizeOperator(CTokenList *paTokens);

  // Parse a string enclosed within certain characters
  // If string is nullptr, parses the string without returning it
  bool ParseString(const c8 chEnclosed, CString *pstr = nullptr);

  // Parse and tokenize a string or a character literal
  // If token list is nullptr, parses the character sequence without tokenizing it
  // If any surrounding character is '\0', doesn't attempt to parse the respective character sequence
  bool TokenizeCharSequence(const c8 chString, const c8 chLiteral, CTokenList *paTokens);

  // Parse an identifier or a keyword
  // If string nullptr, parses the key without returning it
  bool ParseKey(CString *pstr = nullptr);

  // Parse and tokenize a key some common way
  // If token list is nullptr, parses the key without tokenizing it
  bool TokenizeKey(CTokenList *paTokens);

public:
  // Parse a hexadecimal integer and return amount of parsed digits (excluding "0x" prefix)
  // If value is nullptr, parses the integer without returning it
  u32 ParseHexInteger(s64 *piValue = nullptr, bool bHexPrefix = true);

  // Parse an octal integer and return amount of parsed digits (excluding "0o" prefix)
  // If value is nullptr, parses the integer without returning it
  u32 ParseOctInteger(s64 *piValue = nullptr, bool bOctPrefix = true);

  // Parse a binary integer and return amount of parsed digits (excluding "0b" prefix)
  // If value is nullptr, parses the integer without returning it
  u32 ParseBinInteger(s64 *piValue = nullptr, bool bBinPrefix = true);

  // Parse a decimal integer and return amount of parsed digits
  // If value is nullptr, parses the integer without returning it
  u32 ParseDecInteger(s64 *piValue = nullptr);

  // Parse any valid hexadecimal float value and determine whether it may qualify as an integer instead
  // If float value is nullptr, parses the float value without returning it
  // If integer value is specified, sets it to a non-zero value if the parsed float value contained only a whole number
  bool ParseHexFloat(f64 *pfValue = nullptr, s64 *piInteger = nullptr);

  // Hybrid function for parsing and distinguishing between hexadecimal integers and float values
  // If value is nullptr, parses the number without returning it
  bool ParseHexIntegerOrFloat(CVariant *pval = nullptr);

  // Parse any valid decimal float value and determine whether it may qualify as an integer instead
  // If float value is nullptr, parses the float value without returning it
  // If integer value is specified, sets it to a non-zero value if the parsed float value contained only a whole number
  bool ParseDecFloat(f64 *pfValue = nullptr, s64 *piInteger = nullptr);

  // Hybrid function for parsing and distinguishing between decimal integers and float values
  // If value is nullptr, parses the number without returning it
  bool ParseDecIntegerOrFloat(CVariant *pval = nullptr);

  // Parse and tokenize any number from above some common way
  // If token list is nullptr, parses the number without tokenizing it
  bool TokenizeNumber(CTokenList *paTokens);

public:
  // Tokenize a string using all parsers in proper order
  static void Tokenize(CTokenList &aTokens, const CString &str, bool bTokenizeComments = false);
};

NAMESPACE_DREAMY_CLOSE

#endif // (Dreamy Utilities Include Guard)
