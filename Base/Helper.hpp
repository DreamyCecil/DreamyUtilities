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
#include <errno.h>

namespace dreamy
{
  // Resize raw array of a specific type
  template<typename Type> inline
  void ResizeBuffer(Type **pBuffer, size_t iOldSize, size_t iNewSize) {
    Type *pNew = new Type[iNewSize];
    memcpy(pNew, *pBuffer, iOldSize * sizeof(Type));

    delete[] *pBuffer;
    *pBuffer = pNew;
  };

  // Ask yes-or-no question in the console and wait for user input
  inline bool ConsoleYN(const c8 *strQuestion, bool bYesByDefault) {
    // Ask a question and wait for input
    std::cout << strQuestion << (bYesByDefault ? " [Y/n]: " : " [y/N]: ");
    c8 chAnswer = (c8)toupper(getchar());

    if (!bYesByDefault) {
      // Anything but Y is false
      return (chAnswer == 'Y');
    }

    // Anything but N is true
    return (chAnswer != 'N');
  };

  // Get user input from the console
  inline Str_t ConsoleInput(const c8 *strPrompt) {
    Str_t strInput;

    // Display a prompt and wait for input
    std::cout << strPrompt;
    std::getline(std::cin, strInput);

    if (strInput.empty()) return "";

    return strInput.c_str();
  };

  // Wrapper method for opening files (alternative to fopen & fopen_s)
  __forceinline error_t FileOpen(FILE **file, const c8 *strFilename, const c8 *strMode) {
    error_t err = 0; // No error

    #if !_DREAMY_UNIX && _DREAMY_CPP11
      err = fopen_s(file, strFilename, strMode);
    #else
      *file = fopen(strFilename, strMode);
      err = errno;
    #endif

    return err;
  };

  // Simple file copying
  template<typename TypeSrc, typename TypeDst> inline
  void FileCopy(const TypeSrc fileSrc, const TypeDst fileDst) {
    std::ifstream strmSrc(fileSrc, std::ios::binary);
    std::ofstream strmDst(fileDst, std::ios::binary);

    strmDst << strmSrc.rdbuf();
  };

};

#endif // (Dreamy Utilities Include Guard)
