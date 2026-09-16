/* Copyright (c) 2022-2026 Dreamy Cecil
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DREAMYUTILITIES_INCL_FILES_H
#define _DREAMYUTILITIES_INCL_FILES_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

#include "../Types/String.hpp"

#include <fstream>
#include <errno.h>

NAMESPACE_DREAMY_OPEN

// Wrapper method for opening files (alternative to fopen & fopen_s)
__forceinline error_t FileOpen(FILE **file, const c8 *strFilename, const c8 *strMode) {
  #if !_DREAMY_UNIX && _DREAMY_CPP11
    return fopen_s(file, strFilename, strMode);

  #else
    *file = fopen(strFilename, strMode);
    return errno;
  #endif
};

// Simple file copying
template<typename TypeSrc, typename TypeDst>
inline void FileCopy(const TypeSrc fileSrc, const TypeDst fileDst) {
  std::ifstream strmSrc(fileSrc, std::ios::binary);
  std::ofstream strmDst(fileDst, std::ios::binary);

  strmDst << strmSrc.rdbuf();
};

// Simple file copying
inline void FileCopy(const CString &fileSrc, const CString &fileDst) {
  std::ifstream strmSrc(fileSrc.c_str(), std::ios::binary);
  std::ofstream strmDst(fileDst.c_str(), std::ios::binary);

  strmDst << strmSrc.rdbuf();
};

// Check if the file exists
bool FileExists(const c8 *strFileName);

// Check if the file exists
__forceinline bool FileExists(const CString &strFileName) {
  return FileExists(strFileName.c_str());
};

// Get current working directory of the application
CString GetCurrentPath(void);

// Set current working directory of the application
bool SetCurrentPath(const c8 *strPath);

// Set current working directory of the application
__forceinline bool SetCurrentPath(const CString &strPath) {
  return SetCurrentPath(strPath.c_str());
};

// Read text file from a stream into a string
CString ReadTextFile(class CFileDevice &file);

// Open and read text file into a string
CString ReadTextFile(const CString &strFilename);

// Open and read text file into a string if possible
bool ReadTextFileIfPossible(const CString &strFilename, CString &strText);

NAMESPACE_DREAMY_CLOSE

#endif // (Dreamy Utilities Include Guard)
