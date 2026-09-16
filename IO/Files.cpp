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

#include "Files.hpp"
#include "FileDevice.hpp"

#include <iostream>
#include <cctype>
#include <cstdio>
#include <cstdlib>

#if _DREAMY_UNIX
  #include <unistd.h>
#else
  #include <direct.h>
#endif

NAMESPACE_DREAMY_OPEN

bool FileExists(const c8 *strFileName) {
  FILE *file;
  FileOpen(&file, strFileName, "r");

  if (file == nullptr) {
    return false;
  }

  fclose(file);
  return true;
};

CString GetCurrentPath(void) {
  #if _DREAMY_UNIX
    c8 *strPath = getcwd(nullptr, 0);
  #else
    c8 *strPath = _getcwd(nullptr, 0);
  #endif

  if (strPath == nullptr) return ""; // Error

  CString strResult(strPath);
  free(strPath);

  return strResult + "/";
};

bool SetCurrentPath(const c8 *strPath) {
  #if _DREAMY_UNIX
    return chdir(strPath) != -1;
  #else
    return _chdir(strPath) != -1;
  #endif
};

CString ReadTextFile(CFileDevice &file) {
  CString str(file.Size(), '\0');
  file.Read(&str[0], sizeof(c8) * file.Size());

  return str;
};

CString ReadTextFile(const CString &strFilename) {
  CFileDevice file(strFilename.c_str());
  file.Open(IReadWriteDevice::OM_READONLY);

  CString str(file.Size(), '\0');
  file.Read(&str[0], sizeof(c8) * file.Size());

  file.Close();

  return str;
};

bool ReadTextFileIfPossible(const CString &strFilename, CString &strText) {
  CFileDevice file(strFilename.c_str());
  file.Open(IReadWriteDevice::OM_READONLY);

  // Read from the opened file
  if (file.IsOpen()) {
    strText = ReadTextFile(file);
    file.Close();

    return true;
  }

  // Couldn't open the file
  return false;
};

NAMESPACE_DREAMY_CLOSE
