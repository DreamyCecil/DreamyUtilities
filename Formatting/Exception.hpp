//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_FORMATTING_EXCEPTION_H
#define _DREAMYUTILITIES_INCL_FORMATTING_EXCEPTION_H

#include "../Base/Base.hpp"

#include "../Types/Exception.hpp"
#include "../Formatting/Printing.hpp"

namespace dreamy {

// Exception that formatting may throw
class CFormattingException : public CMessageException {

private:
  size_t _char;

public:
  // Default constructor
  CFormattingException(size_t iSetChar) : _char(iSetChar)
  {
    PrintF("Formatting exception at %ull", _char);
  };

  // Get character position
  inline size_t GetCharacter(void) const {
    return _char;
  };
};

};

#endif // (Dreamy Utilities Include Guard)
