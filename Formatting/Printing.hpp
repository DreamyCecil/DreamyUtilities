//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_FORMATTING_PRINTING_H
#define _DREAMYUTILITIES_INCL_FORMATTING_PRINTING_H

#include "../Base/Base.hpp"

#include <stdarg.h>

#if !_DREAMY_UNIX && !_DREAMY_CPP11
  #define vsnprintf _vsnprintf
  #define vswprintf _vsnwprintf
#endif

namespace dreamy {

// Format a string using a list of arguments
inline void VPrintF(Str_t &strOut, const c8 *strFormat, va_list arg) {
  // Allocate new buffer
  size_t iBufferSize = 256;
  c8 *pchBuffer = new c8[iBufferSize];

  // Repeat
  s32 iLength;

  while (true) {
    // Print to the buffer
    iLength = vsnprintf(pchBuffer, iBufferSize, strFormat, arg);

    // Stop if printed ok
    if (iLength != -1) {
      break;
    }

    // Increase the buffer size
    ResizeBuffer_memcpy<c8>(&pchBuffer, iBufferSize, iBufferSize + 256);
    iBufferSize += 256;
  }

  strOut = pchBuffer;
  delete[] pchBuffer;
};

// Reusable inline formatting
#define DREAMY_PRINTF_INLINE(StringOut, StringFormat) { \
  va_list arg; \
  va_start(arg, StringFormat); \
  dreamy::VPrintF(StringOut, StringFormat, arg); \
  va_end(arg); \
}

// Custom string formatting
inline void PrintF(Str_t &strOut, const c8 *strFormat, ...) {
  DREAMY_PRINTF_INLINE(strOut, strFormat);
};

};

#endif // (Dreamy Utilities Include Guard)
