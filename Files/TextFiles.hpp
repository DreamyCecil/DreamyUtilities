//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_TEXTFILES_H
#define _DREAMYUTILITIES_INCL_TEXTFILES_H

#include "../Base/Base.hpp"

#include "../IO/FileDevice.hpp"

namespace dreamy {

// Read text file from a stream into a string
inline CString ReadTextFile(CFileDevice &file) {
  CString str(file.Size(), '\0');
  file.Read(&str[0], sizeof(c8) * file.Size());

  return str;
};

// Open and read text file into a string
inline CString ReadTextFile(const CString &strFilename) {
  CFileDevice file(strFilename.c_str());
  file.Open(IReadWriteDevice::OM_READONLY);

  CString str(file.Size(), '\0');
  file.Read(&str[0], sizeof(c8) * file.Size());

  file.Close();

  return str;
};

// Open and read text file into a string if possible
inline bool ReadTextFileIfPossible(const CString &strFilename, CString &strText) {
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

};

#endif // (Dreamy Utilities Include Guard)
