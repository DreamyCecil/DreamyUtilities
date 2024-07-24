//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_FILENAMES_H
#define _DREAMYUTILITIES_INCL_FILENAMES_H

#include "../Base/Base.hpp"

#include <string>
#include <list>
#include <algorithm>

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
  inline s64 ToS64(void) const {
    s64 i64bit;
    sscanf_s(c_str(), "%lld", &i64bit);

    return i64bit;
  };

  // Convert string into an unsigned 64-bit integer
  inline u64 ToU64(void) const {
    u64 i64bit;
    sscanf_s(c_str(), "%llu", &i64bit);

    return i64bit;
  };

  // Convert ASCII character into lowercase
  static inline c8 CharToLower(c8 ch) {
    return static_cast<c8>(::tolower(static_cast<u8>(ch)));
  };

  // Convert ASCII character into uppercase
  static inline c8 CharToUpper(c8 ch) {
    return static_cast<c8>(::toupper(static_cast<u8>(ch)));
  };

  // Check if two characters are equal (case insensitive)
  static inline bool CompareChars(c8 ch1, c8 ch2) {
    return CharToLower(ch1) == CharToLower(ch2);
  };

  // Convert entire string into lowercase
  inline void ToLower(void) {
    std::transform(begin(), end(), begin(), CharToLower);
  };

  // Convert entire string into uppercase
  inline void ToUpper(void) {
    std::transform(begin(), end(), begin(), CharToUpper);
  };

  // Convert a copy of the string into lowercase and return it
  inline CString AsLower(void) const {
    CString strCopy(*this);
    strCopy.ToLower();
    return strCopy;
  };

  // Convert a copy of the string into uppercase and return it
  inline CString AsUpper(void) const {
    CString strCopy(*this);
    strCopy.ToUpper();
    return strCopy;
  };

  // Replace all occurrences of a character in a string
  inline void Replace(c8 chOld, c8 chNew) {
    std::replace(begin(), end(), chOld, chNew);
  };

  // Check if two strings are equal (case insensitive)
  inline bool Compare(const CString &str) const {
    return size() == str.size() && std::equal(begin(), end(), str.begin(), CompareChars);
  };

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

// Path and filename methods
public:

  // Check if there's a path separator character at some position
  inline bool PathSeparatorAt(size_t i) const {
    return (*this)[i] == '/' || (*this)[i] == '\\';
  };

  // Remove directory from the filename
  inline CString RemoveDir(void) const {
    return substr(find_last_of("/\\") + 1);
  };

  // Remove extension from the filename
  inline CString RemoveExt(void) const {
    const size_t iPeriodPos(find_last_of('.'));
    const size_t iLastDir(find_last_of("/\\"));

    // No period found or it's before the last directory
    if (iPeriodPos == NULL_POS || (iLastDir != NULL_POS && iPeriodPos < iLastDir)) {
      return *this;
    }

    return substr(0, iPeriodPos);
  };

  // Get name of the file
  inline CString GetFileName(void) const {
    return RemoveDir().RemoveExt();
  };

  // Get path to the file
  inline CString GetFileDir(void) const {
    const size_t iLastDirectory(find_last_of("/\\") + 1);
    return substr(0, iLastDirectory);
  };

  // Get file extension with the period
  inline CString GetFileExt(void) const {
    const size_t iPeriodPos(find_last_of('.'));
    const size_t iLastDir(find_last_of("/\\"));

    // No period found or it's before the last directory
    if (iPeriodPos == NULL_POS || (iLastDir != NULL_POS && iPeriodPos < iLastDir)) {
      return "";
    }

    return substr(iPeriodPos);
  };

  // Go up the path until a certain directory
  inline size_t GoUpUntilDir(CString strDirName) const {
    // Convert every string in the same case
    CString strPath = AsLower();
    strDirName.ToLower();

    // Make consistent slashes
    strPath.Replace('\\', '/');

    // Absolute path, e.g. "abc/strDirName/qwe"
    size_t iDir(strPath.rfind("/" + strDirName + "/"));
    if (iDir != NULL_POS) return iDir + 1;

    // Relative down to the desired directory, e.g. "abc/qwe/strDirName"
    iDir = strPath.rfind("/" + strDirName) + 1;
    if (iDir == strPath.length() - strDirName.length()) return iDir;

    // Relative up to the desired directory, e.g. "strDirName/abc/qwe"
    iDir = strPath.find(strDirName + "/");
    if (iDir == 0) return 0;

    // No extra directories up or down the path, must be the same
    if (strPath == strDirName) {
      return 0;
    }

    return NULL_POS;
  };

  // Normalize the path taking "backward" and "current" directories into consideration
  // E.g. "abc/sub1/../sub2/./qwe" -> "abc/sub2/qwe"
  inline void Normalize(void) {
    CString strPath(*this);
    strPath.Replace('\\', '/');

    // Gather parts of the entire path
    std::list<CString> aParts;
    strPath.CharSplit('/', aParts);

    std::list<CString> aFinalPath;
    std::list<CString>::const_iterator it;

    // Iterate through the list of directories
    for (it = aParts.begin(); it != aParts.end(); ++it) {
      const CString &strPart = *it;

      // Ignore current directories
      if (strPart == ".") continue;

      // If encountered a "backward" directory and there are some directories written
      if (strPart == ".." && aFinalPath.size() != 0) {
        // Remove the last directory (go up one directory) and go to the next one
        aFinalPath.pop_back();
        continue;
      }

      // Add directory to the final path
      aFinalPath.push_back(strPart);
    }

    // Reset current path
    *this = "";

    // No path to compose
    if (aFinalPath.size() == 0) return;

    // Compose the final path
    std::list<CString>::const_iterator itLast = --aFinalPath.end();

    for (it = aFinalPath.begin(); it != aFinalPath.end(); ++it) {
      *this += *it;

      // Add separators between the directories
      if (it != itLast) *this += '/';
    }
  };

  // Get length of the root name, if there's any
  size_t RootNameLength() const {
    const size_t ctLen = length();

  #if !_DREAMY_UNIX
    // Starts with a drive letter and a colon on Windows (e.g. "C:")
    const c8 chUpper = CharToUpper((*this)[0]);

    if (ctLen >= 2 && chUpper >= 'A' && chUpper <= 'Z' && (*this)[1] == ':') {
      return 2;
    }
  #endif

    // Starts with a double separator and has any directory right after (e.g. "//abc")
    if (ctLen > 2
     && PathSeparatorAt(0) && PathSeparatorAt(1) && !PathSeparatorAt(2)
     && ::isprint(static_cast<u8>((*this)[2])))
    {
      // Find the next separator, if there's any
      size_t iNextSep = find_first_of("/\\", 3);
      return (iNextSep == NULL_POS ? ctLen : iNextSep);
    }

    return 0;
  };

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
