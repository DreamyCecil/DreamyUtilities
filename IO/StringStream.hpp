//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_STRINGSTREAM_H
#define _DREAMYUTILITIES_INCL_STRINGSTREAM_H

#include "../Base/Base.hpp"

#include "DataStream.hpp"

#include <stdlib.h>

namespace dreamy {

// Class for serializing data as readable text
class CStringStream : public CDataStream {

private:
  CByteArray *_pbaString; // Buffer with characters

public:
  // Default constructor
  CStringStream() : CDataStream()
  {
    _pbaString = new CByteArray('\0', (1 << 16));
    _pDevice = new CBufferDevice(_pbaString);
    _pDevice->Open(IReadWriteDevice::OM_READWRITE);

    _bHasOwnDevice = true;
  };

  // Constructor with a device
  CStringStream(IReadWriteDevice *d, IReadWriteDevice::EOpenMode om) :
    CDataStream(d), _pbaString(nullptr)
  {
    _pDevice->Open(om);
  };

  // Constructor from an existing string
  CStringStream(const c8 *str, size_t iSize) : CDataStream()
  {
    // Create enough bytes and copy the string to the beginning
    _pbaString = new CByteArray('\0', (1 << 16));
    memcpy(_pbaString->Data(), str, iSize);

    _pDevice = new CBufferDevice(_pbaString);
    _pDevice->Open(IReadWriteDevice::OM_WRITEONLY);

    // Go to the end of the string
    Seek(iSize);

    _bHasOwnDevice = true;
  };

  // Return data as a string
  const c8 *GetString(void) const {
    D_ASSERT(_pDevice->GetType() == IReadWriteDevice::TYPE_BUFFER);
    return ((CBufferDevice *)_pDevice)->GetBuffer();
  };

  // Print into the stream
  void PrintF(const c8 *strFormat, ...) {
    CString strOut;
    DREAMY_PRINTF_INLINE(strOut, strFormat);

    *this << strOut;
  };

  // Read a text line until a specific delimiter
  template<typename Type>
  size_t GetLine(Type *strBuffer, size_t iBufferSize, Type chDelimiter = '\n') {
    // Nothing to read
    if (AtEnd()) {
      strBuffer[0] = Type('\0');
      return 0;
    }

    size_t iChars = 0;

    while (true) {
      // Read one character
      Type ch;

      // Pick type-specific operator instead of reading raw data via Read()
      *this >> ch;

      // Reached the delimiter or the end
      if (ch == chDelimiter || AtEnd()) {
        strBuffer[iChars] = Type('\0');
        return iChars;
      }

      // Ignore carriage return characters
      if (ch == Type('\r')) continue;

      // Set character at the latest position
      strBuffer[iChars++] = ch;

      // Reached the limit
      if (iChars == iBufferSize) {
        return iChars;
      }
    }

    return iChars;
  };

// Stream methods
public:

  // Write string into the stream
  virtual CDataStream &operator<<(const c8 *str) {
    // Write every character
    size_t iSize = strlen(str);

    for (size_t i = 0; i < iSize; ++i) {
      *this << str[i];
    }

    return *this;
  };

  // Write STL string into the stream
  virtual CDataStream &operator<<(const CString &str) {
    // Write every character
    size_t iSize = str.length();

    for (size_t i = 0; i < iSize; ++i) {
      *this << str[i];
    }

    return *this;
  };

  virtual CDataStream &operator<<(c8 src) {
    if (Write(&src, 1) != 1) SetStatus(STATUS_WRITEFAILED);
    return *this;
  };

  virtual CDataStream &operator>>(c8 &dst) {
    if (Read(&dst, 1) != 1) dst = 0;
    return *this;
  };

  // Define method for printing a simple value into the stream
  #define WRITE_VAL(_Type, _Format) \
    virtual CDataStream &operator<<(_Type val) { \
      c8 str[128]; \
      sprintf(str, _Format, val); \
      return operator<<(str); \
    };

  // Write methods for numbers
  WRITE_VAL(u8 , "%hhu");
  WRITE_VAL(u16, "%hu");
  WRITE_VAL(u32, "%lu");
  WRITE_VAL(u64, "%llu");
  WRITE_VAL(s8 , "%hhd");
  WRITE_VAL(s16, "%hd");
  WRITE_VAL(s32, "%ld");
  WRITE_VAL(s64, "%lld");
  WRITE_VAL(f32, "%g");
  WRITE_VAL(f64, "%g");
      
  // size_t is not the same as u32/u64 in Unix
  #if _DREAMY_UNIX
  WRITE_VAL(size_t, "%llu");
  #endif

  #undef WRITE_VAL
};

};

#endif // (Dreamy Utilities Include Guard)
