//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_EXCEPTION_H
#define _DREAMYUTILITIES_INCL_EXCEPTION_H
#ifdef _WIN32
  #pragma once
#endif

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

NAMESPACE_DREAMY_OPEN

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
  CString m_strMessage;

public:
  // Default constructor
  CMessageException(const c8 *strError = "") : m_strMessage(strError)
  {
  };

  // Constructor from a string
  CMessageException(const CString &strError) : m_strMessage(strError)
  {
  };

  // Destructor
  virtual ~CMessageException() DREAMY_NOEXCEPT
  {
  };

  // Get error message
  virtual const c8 *What(void) const DREAMY_NOEXCEPT {
    return m_strMessage.c_str();
  };

  // Format an error message
  inline void PrintF(const c8 *strFormat, ...) {
    DREAMY_PRINTF_INLINE(m_strMessage, strFormat);
  };

  // Quick function for throwing message exceptions
  static void Throw(const c8 *strFormat, ...) {
    CMessageException ex;
    DREAMY_PRINTF_INLINE(ex.m_strMessage, strFormat);

    throw ex;
  };
};

// Exception that formatting may throw
class CFormattingException : public CMessageException {

private:
  size_t m_iPos;

public:
  // Default constructor
  CFormattingException(size_t iSetPos, const c8 *strError = "Formatting exception") : m_iPos(iSetPos)
  {
    PrintF("%s at %ull", strError, m_iPos);
  };

  // Get character position
  inline size_t GetPos(void) const {
    return m_iPos;
  };

  // Quick function for throwing token exceptions
  static void Throw(size_t iSetPos, const c8 *strFormat, ...) {
    CString strError;
    DREAMY_PRINTF_INLINE(strError, strFormat);

    throw CFormattingException(iSetPos, strError.c_str());
  };
};

NAMESPACE_DREAMY_CLOSE

#undef PARENT_EXCEPTION

#endif // (Dreamy Utilities Include Guard)
