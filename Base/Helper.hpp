//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_HELPER_H
#define _DREAMYUTILITIES_INCL_HELPER_H

#include "../Base/Base.hpp"

#include <cstring>
#include <iostream>
#include <fstream>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <errno.h>

#if _DREAMY_UNIX
  #include <unistd.h>
#else
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
#endif

namespace dreamy {

// Resize array by constructing values in place and moving the data bytes
template<typename Type> inline
void ResizeBuffer_memcpy(Type **pBuffer, size_t iOldSize, size_t iNewSize) {
  Type *pNew = new Type[iNewSize];
  memcpy(pNew, *pBuffer, iOldSize * sizeof(Type));

  delete[] *pBuffer;
  *pBuffer = pNew;
};

// Resize array by constructing and reassigning values in place
template<typename Type> inline
void ResizeBuffer_new(Type **pBuffer, size_t iOldSize, size_t iNewSize) {
  Type *pNew = new Type[iNewSize];

  // Reassign old values
  while (--iOldSize >= 0) {
    pNew[iOldSize] = (*pBuffer)[iOldSize];
  }

  delete[] *pBuffer;
  *pBuffer = pNew;
};

// Resize array by reallocating C dynamic memory
template<typename Type> inline
void ResizeBuffer_realloc(Type **pBuffer, size_t iNewSize) {
  *pBuffer = (Type *)realloc(*pBuffer, iNewSize * sizeof(Type));
};

// Ask yes-or-no question in the console and wait for user input
inline bool ConsoleYN(const c8 *strQuestion, bool bYesByDefault) {
  CString strInput;

  // Ask a question and wait for input
  std::cout << strQuestion << (bYesByDefault ? " [Y/n]: " : " [y/N]: ");
  std::getline(std::cin, strInput);

  // No answer by default
  c8 chAnswer = 0;

  if (!strInput.empty()) {
    int iUpper = ::toupper(static_cast<u8>(strInput[0]));
    chAnswer = static_cast<c8>(iUpper);
  }

  if (!bYesByDefault) {
    // Anything but Y is false
    return (chAnswer == 'Y');
  }

  // Anything but N is true
  return (chAnswer != 'N');
};

// Get user input from the console
inline CString ConsoleInput(const c8 *strPrompt) {
  CString strInput;

  // Display a prompt and wait for input
  std::cout << strPrompt;
  std::getline(std::cin, strInput);

  if (strInput.empty()) return "";

  return strInput;
};

// Wrapper method for opening files (alternative to fopen & fopen_s)
__forceinline error_t FileOpen(FILE **file, const c8 *strFilename, const c8 *strMode) {
  #if !_DREAMY_UNIX && _DREAMY_CPP11
    return fopen_s(file, strFilename, strMode);

  #else
    *file = fopen(strFilename, strMode);
    return errno;
  #endif
};

// Old standard under GCC cannot accept std::string in stream constructors, so accept them as explicit arguments
#if _DREAMY_UNIX && !_DREAMY_CPP11

// Simple file copying
inline void FileCopy(const CString &fileSrc, const CString &fileDst) {
  std::ifstream strmSrc(fileSrc.c_str(), std::ios::binary);
  std::ofstream strmDst(fileDst.c_str(), std::ios::binary);

  strmDst << strmSrc.rdbuf();
};

#else

// Simple file copying
template<typename TypeSrc, typename TypeDst> inline
void FileCopy(const TypeSrc fileSrc, const TypeDst fileDst) {
  std::ifstream strmSrc(fileSrc, std::ios::binary);
  std::ofstream strmDst(fileDst, std::ios::binary);

  strmDst << strmSrc.rdbuf();
};

#endif

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
__forceinline bool FileExists(const CString &strFileName) {
  return FileExists(strFileName.c_str());
};

// Get current working directory of the application
inline CString GetCurrentPath(void) {
  #if _DREAMY_UNIX
    c8 *strPath = getcwd(nullptr, 0);
    if (strPath == nullptr) return ""; // Error

    CString strResult(strPath);
    free(strPath);

    return strResult + "/";

  #else
    DWORD ctPathLen = GetCurrentDirectoryA(0, 0);
    CString strResult(ctPathLen, '\0');

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
    return SetCurrentDirectoryA(strPath) != 0;
  #endif
};

// Set current working directory of the application
__forceinline bool SetCurrentPath(const CString &strPath) {
  return SetCurrentPath(strPath.c_str());
};

};

#endif // (Dreamy Utilities Include Guard)
