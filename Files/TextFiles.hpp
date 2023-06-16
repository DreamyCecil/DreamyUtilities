//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_TEXTFILES_H
#define _DREAMYUTILITIES_INCL_TEXTFILES_H

#include "../Base/Base.hpp"

#include "../IO/FileDevice.hpp"

namespace dreamy {

// Read text file from a stream into a string
inline Str_t ReadTextFile(CFileDevice &file) {
  Str_t str(file.Size(), '\0');
  file.Read(&str[0], sizeof(c8) * file.Size());

  return str;
};

// Open and read text file into a string
inline Str_t ReadTextFile(const Str_t &strFilename) {
  CFileDevice file(strFilename.c_str());
  file.Open(IReadWriteDevice::OM_READONLY);

  Str_t str(file.Size(), '\0');
  file.Read(&str[0], sizeof(c8) * file.Size());

  file.Close();

  return str;
};

// Open and read text file into a string if possible
inline bool ReadTextFileIfPossible(const Str_t &strFilename, Str_t &strText) {
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
