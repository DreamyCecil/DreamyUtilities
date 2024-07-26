//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_ARRAYS_H
#define _DREAMYUTILITIES_INCL_ARRAYS_H

#include "../DreamyUtilitiesBase.hpp"

#include "String.hpp"

#include <vector>

namespace dreamy {

// Array types
typedef std::vector<bool>    Bits_t;    // Array of bits
typedef std::vector<u8>      Bytes_t;   // Array of bytes
typedef std::vector<s64>     Ints_t;    // Array of integers
typedef std::vector<f64>     Numbers_t; // Array of real numbers
typedef std::vector<CString> Strings_t; // Array of strings

};

#endif // (Dreamy Utilities Include Guard)
