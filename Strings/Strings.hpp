//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_STRINGS_H
#define _DREAMYUTILITIES_INCL_STRINGS_H

#include "../Base/Base.hpp"

#include "../Formatting/Exception.hpp"

#include <vector>
#include <algorithm>

namespace dreamy {

// Convert real number into a string without trailing zeros
template<typename Type> inline
Str_t RealToString(const Type fNumber) {
  #if _DREAMY_CPP11
    return std::to_string(fNumber);

  #else
    // Print the number
    Str_t str;
    {
      c8 strPrint[256];
      snprintf(strPrint, 256, "%f", fNumber);

      str = strPrint;
    }

    // Check for a dot
    size_t iDot = str.find('.');
    if (iDot == NULL_POS) return str;

    size_t iSize = str.length() - 1;

    // Remove zeros from the end
    while (str[iSize] == '0') {
      --iSize;

      // Check for a dot
      if (str[iSize] == '.') {
        return str.substr(0, iSize);
      }
    }

    return str.substr(0, iSize);
  #endif
};

// Convert string into a signed 64-bit integer
inline s64 StrToS64(const Str_t &str) {
  s64 i64bit;
  sscanf_s(str.c_str(), "%lld", &i64bit);

  return i64bit;
};

// Convert string into an unsigned 64-bit integer
inline u64 StrToU64(const Str_t &str) {
  u64 i64bit;
  sscanf_s(str.c_str(), "%llu", &i64bit);

  return i64bit;
};

// Convert character escape sequences into escape characters
inline void ConvertEscapeChars(Str_t &str) {
  Str_t strOriginal = str; // Copy of the string for restoration
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

inline bool CompareChars(c8 ch1, c8 ch2);

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
    if (!CompareChars(*pchMask, *pchCur) && (*pchMask != Type('?') || bEscapeChar)) {
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

// Split a string using a character delimiter
template<typename Type> inline
void CharSplit(const Type &str, const typename Type::value_type chDelimiter, std::vector<Type> &aStrings) {
  size_t iLast = 0;
  size_t iPos = str.find(chDelimiter);

  while (iPos != NULL_POS) {
    aStrings.push_back(str.substr(iLast, iPos - iLast));

    // Skip delimiter
    iPos += 1;

    // Remember position after the delimiter
    iLast = iPos;

    // Get position of the next one
    iPos = str.find(chDelimiter, iPos);
  }

  // Last token
  aStrings.push_back(str.substr(iLast));
};

// Split a string using a string delimiter
template<typename Type> inline
void StringSplit(const Type &str, const Type &strDelimiter, std::vector<Type> &aStrings) {
  const size_t iSize = strDelimiter.Length();

  size_t iLast = 0;
  size_t iPos = str.find(strDelimiter);

  while (iPos != NULL_POS) {
    aStrings.push_back(str.substr(iLast, iPos - iLast));

    // Skip delimiter
    iPos += iSize;

    // Remember position after the delimiter
    iLast = iPos;

    // Get position of the next one
    iPos = str.find(strDelimiter, iPos);
  }

  // Last token
  aStrings.push_back(str.substr(iLast));
};

// Separate a string into multiple arguments (e.g. command line arguments)
// Implemented according to the rules from Microsoft docs:
// https://learn.microsoft.com/en-us/cpp/c-language/parsing-c-command-line-arguments?view=msvc-170
inline void StringToArgs(const c8 *str, std::vector<Str_t> &aArgs, int (*pIsSpace)(int) = &std::isspace) {
  Str_t strCurrent = "";
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

// Convert ASCII character into lowercase
inline c8 CharToLower(c8 ch) {
  return static_cast<c8>(std::tolower(static_cast<u8>(ch)));
};

// Convert ASCII character into uppercase
inline c8 CharToUpper(c8 ch) {
  return static_cast<c8>(std::toupper(static_cast<u8>(ch)));
};

// Check if two characters are equal (case insensitive)
bool CompareChars(c8 ch1, c8 ch2) {
  return CharToLower(ch1) == CharToLower(ch2);
};

// Convert entire string into lowercase
inline void ToLower(Str_t &str) {
  std::transform(str.begin(), str.end(), str.begin(), CharToUpper);
};

// Convert entire string into uppercase
inline void ToUpper(Str_t &str) {
  std::transform(str.begin(), str.end(), str.begin(), CharToUpper);
};

// Convert a copy of the string into lowercase and return it
__forceinline Str_t StrToLower(const Str_t &str) {
  Str_t strCopy(str);
  ToLower(strCopy);
  return strCopy;
};

// Convert a copy of the string into uppercase and return it
__forceinline Str_t StrToUpper(const Str_t &str) {
  Str_t strCopy(str);
  ToUpper(strCopy);
  return strCopy;
};

// Replace all occurrences of a character in a string
inline void Replace(Str_t &str, c8 chOld, c8 chNew) {
  std::replace(str.begin(), str.end(), chOld, chNew);
};

// Check if two strings are equal (case insensitive)
inline bool CompareStrings(const Str_t &str1, const Str_t &str2) {
  return str1.size() == str2.size() && std::equal(str1.begin(), str1.end(), str2.begin(), CompareChars);
};

};

#endif // (Dreamy Utilities Include Guard)
