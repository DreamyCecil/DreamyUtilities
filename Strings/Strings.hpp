//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_STRINGS_H
#define _DREAMYUTILITIES_INCL_STRINGS_H

#include "../Base/Base.hpp"

#include "../Formatting/Exception.hpp"

#include <vector>

namespace dreamy {

// Convert character escape sequences into escape characters
inline void ConvertEscapeChars(CString &str) {
  CString strOriginal = str; // Copy of the string for restoration
  size_t iReplaced = 0; // Amount of replaced characters for offsetting

  // Parse escape characters
  size_t iEscapeSeq = str.find('\\', 0);

  while (iEscapeSeq != NULL_POS) {
    // Erase the escape character
    str.erase(iEscapeSeq, 1);

    // Character after the escape character
    c8 &ch = str[iEscapeSeq];

    switch (ch) {
      // Print the characters themselves
      case '"':
      case '\'':
      case '\\':
        break;

      // Replace special characters with escape characters
      case 'a': ch = '\a'; break;
      case 'b': ch = '\b'; break;
      case 'f': ch = '\f'; break;
      case 'n': ch = '\n'; break;
      case 'r': ch = '\r'; break;
      case 't': ch = '\t'; break;
      case 'v': ch = '\v'; break;

      // Other characters
      default:
        str = strOriginal;
        throw CFormattingException(iEscapeSeq + iReplaced);
    }

    // Find the next escape character
    iEscapeSeq = str.find('\\', iEscapeSeq + 1);
    ++iReplaced;
  }
};

// Convert string up to 8 characters into a 64-bit integer in constant order
__forceinline u64 MultiCharLiteral(const c8 strLiteral[9]) {
  // Empty literal
  if (strLiteral[0] == '\0') return 0;

  u8 iSize = 0;

  // Count length up to 8 characters (9th one is disregarded)
  while (++iSize < 8) {
    if (strLiteral[iSize] == '\0') break;
  }

  // Copy characters
  c8 str[8];
  memcpy(&str[0], &strLiteral[0], iSize);

  // Nullify last characters
  if (iSize != 0) {
    memset(&str[iSize], '\0', 8 - iSize);
  }

  // Return array of characters in constant order as a single integer
  return *(u64 *)(&str[0]);
};

// Compare strings using wildcards
template<typename Type> inline
bool WildcardMatch(const Type *strCheck, const Type *strWildcardMask) {
  bool bWildcard = false; // Discovered a wildcard this time
  bool bEscapeChar = false; // Discovered an escape character

  // Current character in the mask
  const Type *pchMask = strWildcardMask;
  const Type *pchMaskLast = pchMask;

  // Current character in the string
  const Type *pchCur = strCheck;
  const Type *pchCurLast = pchCur;

  while (true) {
    // Discovered "any word" wildcard
    if (*pchMask == Type('*')) {
      // Skip subsequent wildcards
      while (*pchMask == Type('*')) ++pchMask;

      bWildcard = true;

      // Remember current characters
      pchMaskLast = pchMask;
      pchCurLast = pchCur;
    }

    // Reached the end of the mask
    if (*pchMask == Type('\0')) {
      // No more characters in the string as well
      if (*pchCur == Type('\0')) return true;

      // Check for any preceding "any character" wildcards
      for (--pchMask; (pchMask > strWildcardMask) && (*pchMask == Type('?')); --pchMask);

      // Discovered a real wildcard and not an escape sequence
      if (*pchMask == Type('*') && pchMask > strWildcardMask && pchMask[-1] != Type('\\')) {
        return true;
      }

      // Expected a wildcard
      if (!bWildcard) return false;

      pchMask = pchMaskLast;

    // End of the string
    } else if (*pchCur == Type('\0')) {
      // Skip remaining wildcards
      while (*pchMask == Type('*')) ++pchMask;

      // Reached the end of the mask
      return (*pchMask == Type('\0'));
    }

    // Escape character followed by a wildcard character
    if (*pchMask == Type('\\') && (pchMask[1] == Type('*') || pchMask[1] == Type('?'))) {
      ++pchMask;
      bEscapeChar = true;

    } else {
      bEscapeChar = false;
    }

    // Current characters don't match and it's either an "any character" wildcard or an escape sequence
    if (!CString::CompareChars(*pchMask, *pchCur) && (*pchMask != Type('?') || bEscapeChar)) {
      // Expected a wildcard
      if (!bWildcard) return false;

      // Restore characters and proceed with the string
      pchMask = pchMaskLast;
      pchCur = ++pchCurLast;

    } else {
      // Proceed with each string if there are any more characters
      if (*pchMask != Type('\0')) ++pchMask;
      if (*pchCur != Type('\0')) ++pchCur;
    }
  }
};

// Separate a string into multiple arguments (e.g. command line arguments)
// Implemented according to the rules from Microsoft docs:
// https://learn.microsoft.com/en-us/cpp/c-language/parsing-c-command-line-arguments?view=msvc-170
inline void StringToArgs(const c8 *str, std::vector<CString> &aArgs, int (*pIsSpace)(int) = &::isspace) {
  CString strCurrent = "";
  bool bString = false; // String within double quotes

  while (*str != '\0') {
    // Hit a double quote
    if (*str == '"') {
      ++str;

      // A pair of double quotes within a string
      if (bString && *str == '"') {
        // Just add quotes to the argument
        strCurrent += '"';
        ++str;

      // Enter or exit the string
      } else {
        bString = !bString;
      }

    // Hit an escape character
    } else if (*str == '\\') {
      // Within a string
      if (bString) {
        // Skip if it's followed by another one or a quote
        if (str[1] == '\\' || str[1] == '"') {
          ++str;
        }

        // Add the backslash or the escaped character and advance
        strCurrent += *str;
        ++str;

      } else {
        // Check if escape characters preceed a double quote
        bool bUntilQuote = false;
        const c8 *pchCheck = str + 1;

        while (*pchCheck != '\0') {
          // Hit a double quote
          if (*pchCheck == '"') {
            bUntilQuote = true;
            break;

          // Hit something else
          } else if (*pchCheck != '\\') {
            break;
          }

          ++pchCheck;
        }
        
        if (bUntilQuote) {
          // Convert \\ -> \ and \" -> "
          do {
            strCurrent += str[1];
            str += 2;
          } while (*str == '\\');

        } else {
          // Add each espace character
          do {
            strCurrent += *str;
            ++str;
          } while (*str == '\\');
        }
      }

    // Hit an argument separator outside a string
    } else if (!bString && pIsSpace(static_cast<u8>(*str))) {
      // Save current string and reset it
      aArgs.push_back(strCurrent);
      strCurrent = "";

      // Go until the next valid character
      do {
        ++str;
      } while (pIsSpace(static_cast<u8>(*str)));

    // Add any other character to the string and advance
    } else {
      strCurrent += *str;
      ++str;
    }
  }

  // Last argument
  if (strCurrent != "") {
    aArgs.push_back(strCurrent);
  }
};

};

#endif // (Dreamy Utilities Include Guard)
