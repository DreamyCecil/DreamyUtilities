//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#include "String.hpp"
#include "Exception.hpp"
#include "../Data/Memory.hpp"

#include <list>
#include <algorithm>

namespace dreamy {

void CString::VPrintF(const c8 *strFormat, va_list arg) {
  // Allocate new buffer
  size_t iBufferSize = 256;
  c8 *pchBuffer = new c8[iBufferSize];

  // Repeat
  s32 iLength;

  while (true) {
    // Print to the buffer
    iLength = vsnprintf(pchBuffer, iBufferSize, strFormat, arg);

    // Stop if printed ok
    if (iLength != -1) break;

    // Increase the buffer size
    ResizeBuffer_memcpy<c8>(&pchBuffer, iBufferSize, iBufferSize + 256);
    iBufferSize += 256;
  }

  *this = pchBuffer;
  delete[] pchBuffer;
};

void CString::PrintF(const c8 *strFormat, ...) {
  DREAMY_PRINTF_INLINE(*this, strFormat);
};

void CString::ConvertEscapeChars(void) {
  CString strOriginal = *this; // Copy of the string for restoration
  size_t iReplaced = 0; // Amount of replaced characters for offsetting

  // Parse escape characters
  size_t iEscapeSeq = find('\\', 0);

  while (iEscapeSeq != NULL_POS) {
    // Erase the escape character
    CStringBase &str = erase(iEscapeSeq, 1);

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
        *this = strOriginal;
        throw CFormattingException(iEscapeSeq + iReplaced);
    }

    // Find the next escape character
    iEscapeSeq = find('\\', iEscapeSeq + 1);
    ++iReplaced;
  }
};

s64 CString::ToS64(void) const {
  s64 i64bit;
  sscanf_s(c_str(), "%lld", &i64bit);

  return i64bit;
};

u64 CString::ToU64(void) const {
  u64 i64bit;
  sscanf_s(c_str(), "%llu", &i64bit);

  return i64bit;
};

c8 CString::CharToLower(c8 ch) {
  return static_cast<c8>(::tolower(static_cast<u8>(ch)));
};

c8 CString::CharToUpper(c8 ch) {
  return static_cast<c8>(::toupper(static_cast<u8>(ch)));
};

bool CString::CompareChars(c8 ch1, c8 ch2) {
  return CharToLower(ch1) == CharToLower(ch2);
};

void CString::ToLower(void) {
  std::transform(begin(), end(), begin(), CString::CharToLower);
};

void CString::ToUpper(void) {
  std::transform(begin(), end(), begin(), CString::CharToUpper);
};

CString CString::AsLower(void) const {
  CString strCopy(*this);
  strCopy.ToLower();
  return strCopy;
};

CString CString::AsUpper(void) const {
  CString strCopy(*this);
  strCopy.ToUpper();
  return strCopy;
};

void CString::Replace(c8 chOld, c8 chNew) {
  std::replace(begin(), end(), chOld, chNew);
};

bool CString::Compare(const CString &str) const {
  return size() == str.size() && std::equal(begin(), end(), str.begin(), CompareChars);
};

bool CString::StartsWith(const CString &str) const {
  return rfind(str, 0) != NULL_POS;
};

bool CString::EndWith(const CString &str) const {
  const size_t ctThis = length();
  const size_t ctOther = str.length();

  if (ctOther > ctThis) return false;
  return find(str, ctThis - ctOther) != NULL_POS;
};

bool CString::WildcardMatch(const c8 *str, const c8 *strWildcardMask) {
  bool bWildcard = false; // Discovered a wildcard this time
  bool bEscapeChar = false; // Discovered an escape character

  // Current character in the mask
  const c8 *pchMask = strWildcardMask;
  const c8 *pchMaskLast = pchMask;

  // Current character in the string
  const c8 *pchCur = str;
  const c8 *pchCurLast = pchCur;

  while (true) {
    // Discovered "any word" wildcard
    if (*pchMask == '*') {
      // Skip subsequent wildcards
      while (*pchMask == '*') ++pchMask;

      bWildcard = true;

      // Remember current characters
      pchMaskLast = pchMask;
      pchCurLast = pchCur;
    }

    // Reached the end of the mask
    if (*pchMask == '\0') {
      // No more characters in the string as well
      if (*pchCur == '\0') return true;

      // Check for any preceding "any character" wildcards
      for (--pchMask; (pchMask > strWildcardMask) && (*pchMask == '?'); --pchMask);

      // Discovered a real wildcard and not an escape sequence
      if (*pchMask == '*' && pchMask > strWildcardMask && pchMask[-1] != '\\') {
        return true;
      }

      // Expected a wildcard
      if (!bWildcard) return false;

      pchMask = pchMaskLast;

    // End of the string
    } else if (*pchCur == '\0') {
      // Skip remaining wildcards
      while (*pchMask == '*') ++pchMask;

      // Reached the end of the mask
      return (*pchMask == '\0');
    }

    // Escape character followed by a wildcard character
    if (*pchMask == '\\' && (pchMask[1] == '*' || pchMask[1] == '?')) {
      ++pchMask;
      bEscapeChar = true;

    } else {
      bEscapeChar = false;
    }

    // Current characters don't match and it's either an "any character" wildcard or an escape sequence
    if (!CString::CompareChars(*pchMask, *pchCur) && (*pchMask != '?' || bEscapeChar)) {
      // Expected a wildcard
      if (!bWildcard) return false;

      // Restore characters and proceed with the string
      pchMask = pchMaskLast;
      pchCur = ++pchCurLast;

    } else {
      // Proceed with each string if there are any more characters
      if (*pchMask != '\0') ++pchMask;
      if (*pchCur != '\0') ++pchCur;
    }
  }
};

// Check if there's a path separator character at some position
bool CString::PathSeparatorAt(size_t i) const {
  return (*this)[i] == '/' || (*this)[i] == '\\';
};

// Remove directory from the filename
CString CString::RemoveDir(void) const {
  return substr(find_last_of("/\\") + 1);
};

// Remove extension from the filename
CString CString::RemoveExt(void) const {
  const size_t iPeriodPos(find_last_of('.'));
  const size_t iLastDir(find_last_of("/\\"));

  // No period found or it's before the last directory
  if (iPeriodPos == NULL_POS || (iLastDir != NULL_POS && iPeriodPos < iLastDir)) {
    return *this;
  }

  return substr(0, iPeriodPos);
};

// Get name of the file
CString CString::GetFileName(void) const {
  return RemoveDir().RemoveExt();
};

// Get path to the file
CString CString::GetFileDir(void) const {
  const size_t iLastDirectory(find_last_of("/\\") + 1);
  return substr(0, iLastDirectory);
};

// Get file extension with the period
CString CString::GetFileExt(void) const {
  const size_t iPeriodPos(find_last_of('.'));
  const size_t iLastDir(find_last_of("/\\"));

  // No period found or it's before the last directory
  if (iPeriodPos == NULL_POS || (iLastDir != NULL_POS && iPeriodPos < iLastDir)) {
    return "";
  }

  return substr(iPeriodPos);
};

// Go up the path until a certain directory
size_t CString::GoUpUntilDir(CString strDirName) const {
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
// E.g. "abc/sub1/../sub2/./qwe" -> "abc/sub2/qwe" or ".././.." -> "../.."
void CString::Normalize(void) {
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
    // AND it's not another "backward" directory (that has been written because it can't go up any more)
    if (strPart == ".." && !aFinalPath.empty() && aFinalPath.back() != "..") {
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
size_t CString::RootNameLength() const {
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

u64 MultiCharLiteral(const c8 strLiteral[9]) {
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

void StringToArgs(const c8 *str, std::vector<CString> &aArgs, int (*pIsSpace)(int)) {
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

}; // namespace dreamy
