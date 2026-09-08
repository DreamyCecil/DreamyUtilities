//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_CONSOLE_H
#define _DREAMYUTILITIES_INCL_CONSOLE_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

#include "../Types/String.hpp"

NAMESPACE_DREAMY_OPEN

// Ask yes-or-no question in the console and wait for user input
bool ConsoleYN(const c8 *strQuestion, bool bYesByDefault);

// Get user input from the console
CString ConsoleInput(const c8 *strPrompt);

NAMESPACE_DREAMY_CLOSE

#endif // (Dreamy Utilities Include Guard)
