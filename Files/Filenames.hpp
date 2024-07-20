//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_FILENAMES_H
#define _DREAMYUTILITIES_INCL_FILENAMES_H

#include "../Base/Base.hpp"

#include "../Strings/Strings.hpp"

#if _DREAMY_UNIX
  #include <unistd.h>
#else
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
#endif

namespace dreamy {

// String wrapper with helper methods for filenames
class CPath : public Str_t {

// String constructor wrappers
public:
  CPath() : Str_t() {};

  CPath(const Str_t &str) : Str_t(str) {};

  CPath(const Str_t &str, size_t iPos, size_t iSize = NULL_POS) : Str_t(str, iPos, iSize) {};

  CPath(const c8 *str) : Str_t(str) {};

  CPath(const c8 *str, size_t iSize) : Str_t(str, iSize) {};

  CPath(size_t iSize, c8 ch) : Str_t(iSize, ch) {};

  template<typename Iter>
  CPath(Iter itFirst, Iter itLast) : Str_t(itFirst, itLast) {};

#if _DREAMY_CPP11
  CPath(std::initializer_list<c8> il) : Str_t(il) {};

  CPath(Str_t &&str) DREAMY_NOEXCEPT : Str_t(str) {};
#endif

public:
  // Remove directory from the filename
  inline CPath RemoveDir(void) const {
    return substr(find_last_of("/\\") + 1);
  };

  // Remove extension from the filename
  inline CPath RemoveExt(void) const {
    const size_t iPeriodPos(find_last_of('.'));
    const size_t iLastDir(find_last_of("/\\"));

    // No period found or it's before the last directory
    if (iPeriodPos == NULL_POS || (iLastDir != NULL_POS && iPeriodPos < iLastDir)) {
      return *this;
    }

    return substr(0, iPeriodPos);
  };

  // Get name of the file
  inline CPath GetFileName(void) const {
    return RemoveDir().RemoveExt();
  };

  // Get path to the file
  inline CPath GetFileDir(void) const {
    const size_t iLastDirectory(find_last_of("/\\") + 1);
    return substr(0, iLastDirectory);
  };

  // Get file extension with the period
  inline CPath GetFileExt(void) const {
    const size_t iPeriodPos(find_last_of('.'));
    const size_t iLastDir(find_last_of("/\\"));

    // No period found or it's before the last directory
    if (iPeriodPos == NULL_POS || (iLastDir != NULL_POS && iPeriodPos < iLastDir)) {
      return "";
    }

    return substr(iPeriodPos);
  };

  // Go up the path until a certain directory
  inline size_t GoUpUntilDir(Str_t strDirName) const {
    Str_t strPath(*this);

    // Convert every string in the same case
    ToLower(strPath);
    ToLower(strDirName);

    // Make consistent slashes
    Replace(strPath, '\\', '/');

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
};

// Check if the file exists
inline bool FileExists(const c8 *strFileName) {
  FILE *file;
  FileOpen(&file, strFileName, "r");

  if (file == nullptr) {
    return false;
  }

  fclose(file);
  return true;
};

// Check if the file exists
__forceinline bool FileExists(const Str_t &strFileName) {
  return FileExists(strFileName.c_str());
};

// Get current working directory of the application
inline CPath GetCurrentPath(void) {
  #if _DREAMY_UNIX
    c8 *strPath = getcwd(nullptr, 0);
    if (strPath == nullptr) return ""; // Error

    CPath strResult(strPath);
    free(strPath);

    return strResult + "/";

  #else
    size_t ctPathLen = (size_t)GetCurrentDirectoryA(0, 0);
    CPath strResult(ctPathLen, '\0');

    bool bFailed = (GetCurrentDirectoryA(ctPathLen, &strResult[0]) == 0);
    if (bFailed) return ""; // Error

    // Last character is set to null, so replace it with another separator
    strResult[ctPathLen - 1] = '\\';

    return strResult;
  #endif
};

// Set current working directory of the application
inline bool SetCurrentPath(const c8 *strPath) {
  #if _DREAMY_UNIX
    return chdir(strPath) != -1;
  #else
    return SetCurrentDirectoryA(strPath);
  #endif
};

// Set current working directory of the application
__forceinline bool SetCurrentPath(const Str_t &strPath) {
  return SetCurrentPath(strPath.c_str());
};

};

#endif // (Dreamy Utilities Include Guard)
