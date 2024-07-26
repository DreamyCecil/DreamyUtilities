//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_STRING_H
#define _DREAMYUTILITIES_INCL_STRING_H

#include "../DreamyUtilitiesBase.hpp"

#include <string>
#include <vector>

namespace dreamy {

// Base class alias so as not to anger the ancients (i.e. MSVC 6.0)
typedef std::string CStringBase;

// String wrapper with helper methods for filenames
class CString : public CStringBase {

// String constructor wrappers
public:
  CString() : CStringBase() {};

  CString(const CStringBase &str) : CStringBase(str) {};

  CString(const CStringBase &str, size_t iPos, size_t iSize = NULL_POS) : CStringBase(str, iPos, iSize) {};

  CString(const c8 *str) : CStringBase(str) {};

  CString(const c8 *str, size_t iSize) : CStringBase(str, iSize) {};

  CString(size_t iSize, c8 ch) : CStringBase(iSize, ch) {};

  template<typename Iter>
  CString(Iter itFirst, Iter itLast) : CStringBase(itFirst, itLast) {};

#if _DREAMY_CPP11
  CString(std::initializer_list<c8> il) : CStringBase(il) {};

  CString(CStringBase &&str) DREAMY_NOEXCEPT : CStringBase(str) {};
#endif

// Generic methods
public:

  // Format a string using a list of arguments
  void VPrintF(const c8 *strFormat, va_list arg);

  // Custom string formatting
  void PrintF(const c8 *strFormat, ...);

  // Convert character escape sequences into escape characters
  void ConvertEscapeChars(void);

  // Convert real number into a string without trailing zeros
  template<typename Type>
  void FromReal(const Type fNumber) {
    #if _DREAMY_CPP11
      *this = std::to_string(fNumber);

    #else
      // Print the number
      c8 strPrint[256];
      snprintf(strPrint, 256, "%f", fNumber);
      *this = strPrint;

      // Check for a dot
      size_t iDot = find('.');
      if (iDot == NULL_POS) return;

      CString str(*this);
      size_t iSize = str.length() - 1;

      // Remove zeros from the end
      while (str[iSize] == '0') {
        --iSize;

        // Check for a dot
        if (str[iSize] == '.') break;
      }

      *this = str.substr(0, iSize);
    #endif
  };

  // Convert string into a signed 64-bit integer
  s64 ToS64(void) const;

  // Convert string into an unsigned 64-bit integer
  u64 ToU64(void) const;

  // Convert ASCII character into lowercase
  static c8 CharToLower(c8 ch);

  // Convert ASCII character into uppercase
  static c8 CharToUpper(c8 ch);

  // Check if two characters are equal (case insensitive)
  static bool CompareChars(c8 ch1, c8 ch2);

  // Convert entire string into lowercase
  void ToLower(void);

  // Convert entire string into uppercase
  void ToUpper(void);

  // Convert a copy of the string into lowercase and return it
  CString AsLower(void) const;

  // Convert a copy of the string into uppercase and return it
  CString AsUpper(void) const;

  // Replace all occurrences of a character in a string
  void Replace(c8 chOld, c8 chNew);

  // Check if two strings are equal (case-insensitive)
  bool Compare(const CString &str) const;

  // Check if a string starts with a substring
  bool StartsWith(const CString &str) const;

  // Check if a string ends with a substring
  bool EndWith(const CString &str) const;

  // Split a string using a character delimiter
  template<typename TypeContainer>
  void CharSplit(const c8 chDelimiter, TypeContainer &aStrings) const {
    size_t iLast = 0;
    size_t iPos = find(chDelimiter);

    while (iPos != NULL_POS) {
      aStrings.push_back(substr(iLast, iPos - iLast));

      // Skip delimiter
      iPos += 1;

      // Remember position after the delimiter
      iLast = iPos;

      // Get position of the next one
      iPos = find(chDelimiter, iPos);
    }

    // Last token
    aStrings.push_back(substr(iLast));
  };

  // Split a string using a string delimiter
  template<typename TypeContainer>
  void StringSplit(const CString &strDelimiter, TypeContainer &aStrings) const {
    const size_t iSize = strDelimiter.length();

    size_t iLast = 0;
    size_t iPos = find(strDelimiter);

    while (iPos != NULL_POS) {
      aStrings.push_back(substr(iLast, iPos - iLast));

      // Skip delimiter
      iPos += iSize;

      // Remember position after the delimiter
      iLast = iPos;

      // Get position of the next one
      iPos = find(strDelimiter, iPos);
    }

    // Last token
    aStrings.push_back(substr(iLast));
  };

  // Compare strings using wildcards
  static bool WildcardMatch(const c8 *str, const c8 *strWildcardMask);

  // Compare strings using wildcards
  inline bool WildcardMatch(const c8 *strWildcardMask) const {
    return WildcardMatch(c_str(), strWildcardMask);
  };

// Path and filename methods
public:

  // Check if there's a path separator character at some position
  bool PathSeparatorAt(size_t i) const;

  // Remove directory from the filename
  CString RemoveDir(void) const;

  // Remove extension from the filename
  CString RemoveExt(void) const;

  // Get name of the file
  CString GetFileName(void) const;

  // Get path to the file
  CString GetFileDir(void) const;

  // Get file extension with the period
  CString GetFileExt(void) const;

  // Go up the path until a certain directory
  size_t GoUpUntilDir(CString strDirName) const;

  // Normalize the path taking "backward" and "current" directories into consideration
  // E.g. "abc/sub1/../sub2/./qwe" -> "abc/sub2/qwe"
  void Normalize(void);

  // Get length of the root name, if there's any
  size_t RootNameLength() const;

  // Check if the path starts with a root name (e.g. "C:" or "//abc")
  inline bool HasRootName() const {
    return RootNameLength() != 0;
  };

  // Check if the path starts with a root directory (e.g. "C:/" or "//abc/")
  inline bool HasRootDirectory() const {
    const size_t ctRoot = RootNameLength();
    return length() > ctRoot && PathSeparatorAt(ctRoot);
  };

  // Check if it's a full path
  inline bool IsAbsolute() const {
  #if _DREAMY_UNIX
    return HasRootDirectory();
  #else
    return HasRootName() && HasRootDirectory();
  #endif
  };

  // Check if it's a relative path
  inline bool IsRelative() const {
    return !IsAbsolute();
  };
};

// Reusable inline formatting
#define DREAMY_PRINTF_INLINE(StringOut, StringFormat) { \
  va_list arg; \
  va_start(arg, StringFormat); \
  (StringOut).VPrintF(StringFormat, arg); \
  va_end(arg); \
}

// Convert string up to 8 characters into a 64-bit integer in constant order
u64 MultiCharLiteral(const c8 strLiteral[9]);

// Separate a string into multiple arguments (e.g. command line arguments)
// Implemented according to the rules from Microsoft docs:
// https://learn.microsoft.com/en-us/cpp/c-language/parsing-c-command-line-arguments?view=msvc-170
void StringToArgs(const c8 *str, std::vector<CString> &aArgs, int (*pIsSpace)(int) = &::isspace);

};

#if _DREAMY_CPP11
namespace std {

// String class hasher
template<>
struct hash<dreamy::CString>
{
  size_t operator()(const dreamy::CString &str) const {
    return hash<dreamy::CStringBase>()(str);
  }
};

};
#endif

#endif // (Dreamy Utilities Include Guard)
