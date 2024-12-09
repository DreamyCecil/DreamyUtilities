//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#include "DataStream.hpp"

#include "../Data/Endian.hpp"
#include "../Types/Exception.hpp"
#include "../IO/BufferDevice.hpp"

namespace dreamy {

CDataStream::CDataStream() : _pDevice(nullptr),
  _eByteOrder(BO_PLATFORM), _eStatus(STATUS_OK),
  _bExceptionMode(false), _bHasOwnDevice(false)
{
};

CDataStream::CDataStream(IReadWriteDevice *d) : _pDevice(d),
  _eByteOrder(BO_PLATFORM), _eStatus(STATUS_OK),
  _bExceptionMode(false), _bHasOwnDevice(false)
{
};

CDataStream::CDataStream(const CByteArray &ba) :
  _eByteOrder(BO_PLATFORM), _eStatus(STATUS_OK),
  _bExceptionMode(false), _bHasOwnDevice(true)
{
  _pDevice = new CBufferDevice(const_cast<CByteArray *>(&ba));
  _pDevice->Open(IReadWriteDevice::OM_READONLY);
};

CDataStream::CDataStream(CByteArray *pba, IReadWriteDevice::EOpenMode om) :
  _eByteOrder(BO_PLATFORM), _eStatus(STATUS_OK),
  _bExceptionMode(false), _bHasOwnDevice(true)
{
  _pDevice = new CBufferDevice(pba);
  _pDevice->Open(om);
};

CDataStream::~CDataStream()
{
  if (_bHasOwnDevice) {
    delete _pDevice;
  }
};

bool CDataStream::AtEnd(void) const {
  return (_eStatus == STATUS_READPASTEND || _pDevice == nullptr || _pDevice->AtEnd());
};

void CDataStream::SetDevice(IReadWriteDevice *d) {
  if (_bHasOwnDevice) {
    delete _pDevice;
    _bHasOwnDevice = false;
  }

  _pDevice = d;
};

void CDataStream::SetStatus(const EStatus eStatus) {
  // Not OK
  if (_eStatus != STATUS_OK) return;

  _eStatus = eStatus;

  // Don't throw exceptions
  if (!_bExceptionMode) return;

  switch (eStatus) {
    case STATUS_READPASTEND: throw CMessageException("STATUS_READPASTEND"); break;
    case STATUS_WRITEFAILED: throw CMessageException("STATUS_WRITEFAILED"); break;
    default: break;
  }
};

size_t CDataStream::Read(void *pBuffer, size_t iLength) {
  if (_eStatus != STATUS_OK || pBuffer == nullptr || Device() == nullptr) {
    return NULL_POS;
  }

  const size_t iResult = Device()->Read((c8 *)pBuffer, iLength);

  if (iResult != iLength) {
    SetStatus(STATUS_READPASTEND);
  }

  return iResult;
};

CByteArray CDataStream::Read(size_t iLength) {
  CByteArray baResult;

  baResult.Resize(iLength);
  Read(baResult.Data(), iLength);

  return baResult;
};

size_t CDataStream::Write(const void *pData, size_t iLength) {
  if (_eStatus != STATUS_OK || pData == nullptr || Device() == nullptr) {
    return NULL_POS;
  }

  return Device()->Write((const c8 *)pData, iLength);
};

size_t CDataStream::Write(const CByteArray &baData) {
  if (baData.IsNull()) return NULL_POS;

  return Write(baData.ConstData(), baData.Size());
};

size_t CDataStream::Pos(void) const {
  return _pDevice->Pos();
};

bool CDataStream::Seek(size_t iOffset) {
  if (_eStatus != STATUS_OK) return false;

  bool bResult = Device()->Seek(iOffset);

  if (!bResult) {
    SetStatus(STATUS_READPASTEND);
  }

  return bResult;
};

size_t CDataStream::Skip(size_t iLength) {
  if (_eStatus != STATUS_OK) return NULL_POS;

  const size_t iResult = Device()->Skip(iLength);

  if (iResult != iLength) {
    SetStatus(STATUS_READPASTEND);
  }

  return iResult;
};

size_t CDataStream::Peek(void *pBuffer, size_t iLength) {
  if (_eStatus != STATUS_OK) return NULL_POS;

  const size_t iResult = Device()->Peek((c8 *)pBuffer, iLength);

  if (iResult != iLength) {
    SetStatus(STATUS_READPASTEND);
  }

  return iResult;
};

CByteArray CDataStream::Peek(size_t iLength) {
  CByteArray baResult;

  baResult.Resize(iLength);
  Peek(baResult.Data(), iLength);

  return baResult;
};

bool CDataStream::Expect(const CByteArray &baData) {
  CByteArray baResult = Read(baData.Size());
      
  // Mismatching bytes
  if (baResult != baData) {
    CMessageException::Throw("Expected '%s' sequence of bytes at 0x%X but got '%s'", baData.ConstData(), Pos(), baResult.ConstData());
    return false;
  }

  return true;
};

// Change byte order of the value before writing/after reading
#define CHANGE_BYTE_ORDER(_Value) \
  if (GetByteOrder() == BO_LITTLEENDIAN) { \
    _Value = endian::ToLittle(_Value); \
  } else { \
    _Value = endian::ToBig(_Value); \
  }

CDataStream &CDataStream::operator>>(u8 &dst) {
  if (Read(&dst, 1) != 1) dst = 0;
  return *this;
};

CDataStream &CDataStream::operator>>(u16 &dst) {
  if (Read(&dst, 2) != 2) dst = 0;
  CHANGE_BYTE_ORDER(dst);
  return *this;
};

CDataStream &CDataStream::operator>>(u32 &dst) {
  if (Read(&dst, 4) != 4) dst = 0;
  CHANGE_BYTE_ORDER(dst);
  return *this;
};

CDataStream &CDataStream::operator>>(u64 &dst) {
  if (Read(&dst, 8) != 8) dst = 0;
  CHANGE_BYTE_ORDER(dst);
  return *this;
};

CDataStream &CDataStream::operator>>(s8 &dst) {
  if (Read(&dst, 1) != 1) dst = 0;
  return *this;
};

CDataStream &CDataStream::operator>>(s16 &dst) {
  if (Read(&dst, 2) != 2) dst = 0;
  CHANGE_BYTE_ORDER(dst);
  return *this;
};

CDataStream &CDataStream::operator>>(s32 &dst) {
  if (Read(&dst, 4) != 4) dst = 0;
  CHANGE_BYTE_ORDER(dst);
  return *this;
};

CDataStream &CDataStream::operator>>(s64 &dst) {
  if (Read(&dst, 8) != 8) dst = 0;
  CHANGE_BYTE_ORDER(dst);
  return *this;
};

CDataStream &CDataStream::operator>>(f32 &dst) {
  if (Read(&dst, 4) != 4) dst = 0;
  CHANGE_BYTE_ORDER(dst);
  return *this;
};

CDataStream &CDataStream::operator>>(f64 &dst) {
  if (Read(&dst, 8) != 8) dst = 0;
  CHANGE_BYTE_ORDER(dst);
  return *this;
};

CDataStream &CDataStream::operator<<(u8 src) { 
  if (Write(&src, 1) != 1) SetStatus(STATUS_WRITEFAILED);
  return *this;
};

CDataStream &CDataStream::operator<<(u16 src) {
  CHANGE_BYTE_ORDER(src);
  if (Write(&src, 2) != 2) SetStatus(STATUS_WRITEFAILED);
  return *this;
};

CDataStream &CDataStream::operator<<(u32 src) {
  CHANGE_BYTE_ORDER(src);
  if (Write(&src, 4) != 4) SetStatus(STATUS_WRITEFAILED);
  return *this;
};

CDataStream &CDataStream::operator<<(u64 src) {
  CHANGE_BYTE_ORDER(src);
  if (Write(&src, 8) != 8) SetStatus(STATUS_WRITEFAILED);
  return *this;
};

CDataStream &CDataStream::operator<<(s8 src) {
  if (Write(&src, 1) != 1) SetStatus(STATUS_WRITEFAILED);
  return *this;
};

CDataStream &CDataStream::operator<<(s16 src) {
  CHANGE_BYTE_ORDER(src);
  if (Write(&src, 2) != 2) SetStatus(STATUS_WRITEFAILED);
  return *this;
};

CDataStream &CDataStream::operator<<(s32 src) {
  CHANGE_BYTE_ORDER(src);
  if (Write(&src, 4) != 4) SetStatus(STATUS_WRITEFAILED);
  return *this;
};

CDataStream &CDataStream::operator<<(s64 src) {
  CHANGE_BYTE_ORDER(src);
  if (Write(&src, 8) != 8) SetStatus(STATUS_WRITEFAILED);
  return *this;
};

CDataStream &CDataStream::operator<<(f32 src) {
  CHANGE_BYTE_ORDER(src);
  if (Write(&src, 4) != 4) SetStatus(STATUS_WRITEFAILED);
  return *this;
};

CDataStream &CDataStream::operator<<(f64 src) {
  CHANGE_BYTE_ORDER(src);
  if (Write(&src, 8) != 8) SetStatus(STATUS_WRITEFAILED);
  return *this;
};

#if _DREAMY_UNIX

CDataStream &CDataStream::operator<<(size_t src) {
  CHANGE_BYTE_ORDER(src);
  if (Write(&src, sizeof(size_t)) != sizeof(size_t)) SetStatus(STATUS_WRITEFAILED);
  return *this;
};

CDataStream &CDataStream::operator>>(size_t &dst) {
  if (Read(&dst, sizeof(size_t)) != sizeof(size_t)) dst = 0;
  CHANGE_BYTE_ORDER(dst);
  return *this;
};

#endif

CDataStream &CDataStream::operator<<(c8 src) {
  if (Write(&src, 1) != 1) SetStatus(STATUS_WRITEFAILED);
  return *this;
};

CDataStream &CDataStream::operator>>(c8 &dst) {
  if (Read(&dst, 1) != 1) dst = 0;
  return *this;
};

CDataStream &CDataStream::operator<<(const CByteArray &baData) {
  // Write length
  const u32 ulDataLength = (u32)baData.Size();
  *this << ulDataLength;

  // Write bytes if there are any
  if (ulDataLength != 0 && Write(baData.ConstData(), ulDataLength) != ulDataLength) {
    SetStatus(STATUS_WRITEFAILED);
  }

  return *this;
};

CDataStream &CDataStream::operator>>(CByteArray &baData) {
  // Read length
  u32 ulDataLength;
  *this >> ulDataLength;

  baData.Clear();
  baData.Resize(ulDataLength);

  // Read bytes if there are any written
  if (ulDataLength != 0 && Read(baData.Data(), ulDataLength) != ulDataLength) {
    memset(baData.Data(), 0, ulDataLength);
  }

  return *this;
};

CDataStream &CDataStream::operator<<(const CString &str) {
  // Write length
  size_t iSize = str.length();
  *this << (u32)iSize;

  // Write every character
  for (size_t i = 0; i < iSize; ++i) {
    *this << str[i];
  }

  return *this;
};

CDataStream &CDataStream::operator>>(CString &str) {
  // Read length
  u32 iSize;
  *this >> iSize;

  // Read every character into the string
  str.resize(iSize);

  if (Read(&str[0], iSize) != iSize) {
    memset(&str[0], '\0', iSize);
  }

  return *this;
};

CDataStream &CDataStream::operator<<(const c8 *str) {
  // Write length
  size_t iSize = strlen(str);
  *this << (u32)iSize;

  // Write every character
  for (size_t i = 0; i < iSize; ++i) {
    *this << str[i];
  }

  return *this;
};

CDataStream &CDataStream::operator>>(c8 *str) {
  // Read length
  u32 iSize;
  *this >> iSize;

  // Read every character into the string
  if (Read(&str[0], iSize) != iSize) {
    memset(&str[0], '\0', iSize);
  }

  return *this;
};

}; // namespace dreamy
