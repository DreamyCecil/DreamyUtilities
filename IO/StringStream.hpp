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

#ifndef _DREAMYUTILITIES_INCL_STRINGSTREAM_H
#define _DREAMYUTILITIES_INCL_STRINGSTREAM_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

#include "DataStream.hpp"

NAMESPACE_DREAMY_OPEN

// Class for serializing data as readable text
class CStringStream : public CDataStream {

private:
  CByteArray *m_pbaString; // Buffer with characters

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

  // Write methods for numbers
  virtual CDataStream &operator<<(u8 src);
  virtual CDataStream &operator<<(u16 src);
  virtual CDataStream &operator<<(u32 src);
  virtual CDataStream &operator<<(u64 src);
  virtual CDataStream &operator<<(s8 src);
  virtual CDataStream &operator<<(s16 src);
  virtual CDataStream &operator<<(s32 src);
  virtual CDataStream &operator<<(s64 src);
  virtual CDataStream &operator<<(f32 src);
  virtual CDataStream &operator<<(f64 src);

  // size_t is not the same as u32/u64 in Unix
  #if _DREAMY_UNIX
  virtual CDataStream &operator<<(size_t src);
  #endif
};

NAMESPACE_DREAMY_CLOSE

#endif // (Dreamy Utilities Include Guard)
