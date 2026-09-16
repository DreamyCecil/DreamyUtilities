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

#include "Console.hpp"

#include <iostream>

NAMESPACE_DREAMY_OPEN

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

NAMESPACE_DREAMY_CLOSE
