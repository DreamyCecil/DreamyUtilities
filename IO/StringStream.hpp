//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_STRINGSTREAM_H
#define _DREAMYUTILITIES_INCL_STRINGSTREAM_H

#include "../DreamyUtilitiesBase.hpp"

#include "DataStream.hpp"

namespace dreamy {

// Class for serializing data as readable text
class CStringStream : public CDataStream {

private:
  CByteArray *_pbaString; // Buffer with characters

public:
  // Default constructor
  CStringStream();

  // Constructor with a device
  CStringStream(IReadWriteDevice *d, IReadWriteDevice::EOpenMode om);

  // Constructor from an existing string
  CStringStream(const c8 *str, size_t iSize);

  // Return data as a string
  const c8 *GetString(void) const;

  // Print into the stream
  void PrintF(const c8 *strFormat, ...);

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
  virtual CDataStream &operator<<(const c8 *str);

  // Write STL string into the stream
  virtual CDataStream &operator<<(const CString &str);

  virtual CDataStream &operator<<(c8 src);
  virtual CDataStream &operator>>(c8 &dst);

  // Declare method for printing a simple value into the stream
  #define WRITE_VAL(_Type) virtual CDataStream &operator<<(_Type val)

  // Write methods for numbers
  WRITE_VAL(u8);
  WRITE_VAL(u16);
  WRITE_VAL(u32);
  WRITE_VAL(u64);
  WRITE_VAL(s8);
  WRITE_VAL(s16);
  WRITE_VAL(s32);
  WRITE_VAL(s64);
  WRITE_VAL(f32);
  WRITE_VAL(f64);

  // size_t is not the same as u32/u64 in Unix
  #if _DREAMY_UNIX
    WRITE_VAL(size_t);
  #endif

  #undef WRITE_VAL
};

};

#endif // (Dreamy Utilities Include Guard)
