//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#include "Console.hpp"

#include <iostream>

namespace dreamy {

bool ConsoleYN(const c8 *strQuestion, bool bYesByDefault) {
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

CString ConsoleInput(const c8 *strPrompt) {
  CString strInput;

  // Display a prompt and wait for input
  std::cout << strPrompt;
  std::getline(std::cin, strInput);

  if (strInput.empty()) return "";

  return strInput;
};

};
