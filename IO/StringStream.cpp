//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#include "StringStream.hpp"
#include "BufferDevice.hpp"

#include <cstdlib>

namespace dreamy {

CStringStream::CStringStream() : CDataStream()
{
  _pbaString = new CByteArray('\0', (1 << 16));
  _pDevice = new CBufferDevice(_pbaString);
  _pDevice->Open(IReadWriteDevice::OM_READWRITE);

  _bHasOwnDevice = true;
};

CStringStream::CStringStream(IReadWriteDevice *d, IReadWriteDevice::EOpenMode om) :
  CDataStream(d), _pbaString(nullptr)
{
  _pDevice->Open(om);
};

CStringStream::CStringStream(const c8 *str, size_t iSize) : CDataStream()
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

const c8 *CStringStream::GetString(void) const {
  D_ASSERT(_pDevice->GetType() == IReadWriteDevice::TYPE_BUFFER);
  return ((CBufferDevice *)_pDevice)->GetBuffer();
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

};
