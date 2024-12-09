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
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
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

bool SetCurrentPath(const c8 *strPath) {
  #if _DREAMY_UNIX
    return chdir(strPath) != -1;
  #else
    return SetCurrentDirectoryA(strPath) != 0;
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
