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

#include "StringStream.hpp"
#include "BufferDevice.hpp"

#include <cstdlib>

NAMESPACE_DREAMY_OPEN

CStringStream::CStringStream() : CDataStream()
{
  m_pbaString = new CByteArray('\0', (1 << 16));
  m_pDevice = new CBufferDevice(m_pbaString);
  m_pDevice->Open(IReadWriteDevice::OM_READWRITE);

  m_bHasOwnDevice = true;
};

CStringStream::CStringStream(IReadWriteDevice *d, IReadWriteDevice::EOpenMode om) :
  CDataStream(d), m_pbaString(nullptr)
{
  m_pDevice->Open(om);
};

CStringStream::CStringStream(const c8 *str, size_t iSize) : CDataStream()
{
  // Create enough bytes and copy the string to the beginning
  m_pbaString = new CByteArray('\0', (1 << 16));
  memcpy(m_pbaString->Data(), str, iSize);

  m_pDevice = new CBufferDevice(m_pbaString);
  m_pDevice->Open(IReadWriteDevice::OM_WRITEONLY);

  // Go to the end of the string
  Seek(iSize);

  m_bHasOwnDevice = true;
};

const c8 *CStringStream::GetString(void) const {
  D_ASSERT(m_pDevice->GetType() == IReadWriteDevice::TYPE_BUFFER);
  return ((CBufferDevice *)m_pDevice)->GetBuffer();
};

void CStringStream::PrintF(const c8 *strFormat, ...) {
  CString strOut;
  DREAMY_PRINTF_INLINE(strOut, strFormat);

  *this << strOut;
};

CDataStream &CStringStream::operator<<(const c8 *str) {
  // Write every character
  size_t iSize = strlen(str);

  for (size_t i = 0; i < iSize; ++i) {
    *this << str[i];
  }

  return *this;
};

CDataStream &CStringStream::operator<<(const CString &str) {
  // Write every character
  size_t iSize = str.length();

  for (size_t i = 0; i < iSize; ++i) {
    *this << str[i];
  }

  return *this;
};

CDataStream &CStringStream::operator<<(c8 src) {
  if (Write(&src, 1) != 1) SetStatus(STATUS_WRITEFAILED);
  return *this;
};

CDataStream &CStringStream::operator>>(c8 &dst) {
  if (Read(&dst, 1) != 1) dst = 0;
  return *this;
};

// Define method for printing a simple value into the stream
#define WRITE_VAL(_Type, _Format) \
  CDataStream &CStringStream::operator<<(_Type val) { \
    c8 str[128]; \
    sprintf(str, _Format, val); \
    return operator<<(str); \
  };

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

#if _DREAMY_UNIX
  WRITE_VAL(size_t, "%llu");
#endif

#undef WRITE_VAL

NAMESPACE_DREAMY_CLOSE
