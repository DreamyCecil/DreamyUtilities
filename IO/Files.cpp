//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

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

namespace dreamy {

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

}; // namespace dreamy
