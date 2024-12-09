//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_EXCEPTION_H
#define _DREAMYUTILITIES_INCL_EXCEPTION_H

#include "../DreamyUtilitiesBase.hpp"

#include "String.hpp"

#if defined(_DREAMY_STL_EXCEPTION)
  #include <exception>

  // Derive generic exception from std::exception
  #define PARENT_EXCEPTION : public std::exception

#else
  // No parent exception class
  #define PARENT_EXCEPTION
#endif

namespace dreamy {

// Generic exception type for all framework exceptions
class CException PARENT_EXCEPTION {

public:
  // Destructor
  virtual ~CException() DREAMY_NOEXCEPT
  {
  };

  // Get error message
  virtual const c8 *What(void) const DREAMY_NOEXCEPT {
    return "Generic dreamy::CException";
  };

  // Get error message STL-style (in case it's derived from std::exception)
  virtual const c8 *what(void) const DREAMY_NOEXCEPT final {
    return What();
  };
};

// Generic exception with a message (framework's version of std::runtime_error)
class CMessageException : public CException {

protected:
  CString _message;

public:
  // Default constructor
  CMessageException(const c8 *strError = "") : _message(strError)
  {
  };

  // Constructor from a string
  CMessageException(const CString &strError) : _message(strError)
  {
  };

  // Destructor
  virtual ~CMessageException() DREAMY_NOEXCEPT
  {
  };

  // Get error message
  virtual const c8 *What(void) const DREAMY_NOEXCEPT {
    return _message.c_str();
  };

  // Format an error message
  inline void PrintF(const c8 *strFormat, ...) {
    DREAMY_PRINTF_INLINE(_message, strFormat);
  };

  // Quick function for throwing message exceptions
  static void Throw(const c8 *strFormat, ...) {
    CMessageException ex;
    DREAMY_PRINTF_INLINE(ex._message, strFormat);

    throw ex;
  };
};

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

}; // namespace dreamy

#undef PARENT_EXCEPTION

#endif // (Dreamy Utilities Include Guard)
