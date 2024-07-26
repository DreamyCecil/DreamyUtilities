//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#include "BufferDevice.hpp"
#include "../Math/Algorithm.hpp"

namespace dreamy {

CBufferDevice::CBufferDevice() : _pData(nullptr), _iPos(0)
{
};

CBufferDevice::CBufferDevice(CByteArray *pByteArray) : _pData(pByteArray), _iPos(0)
{
};

bool CBufferDevice::Open(EOpenMode eOpenMode) {
  if (_pData == nullptr) {
    return false;
  }

  _eOpenMode = eOpenMode;
  _iPos = 0;
  return IsOpen();
};

void CBufferDevice::Close(void) {
  _eOpenMode = OM_UNOPEN;
  _iPos = 0;
};

size_t CBufferDevice::Pos(void) const {
  return IsOpen() ? _iPos : NULL_POS;
};

bool CBufferDevice::AtEnd(void) const {
  return Pos() >= Size();
};

size_t CBufferDevice::Size(void) const {
  return _pData->Size();
};

bool CBufferDevice::Seek(size_t iOffset) {
  if (_pData == nullptr || IsOpen()) {
    return false;
  }

  // Past the limit
  if (iOffset >= Size()) return false;

  // Set new position
  _iPos = iOffset;
  return true;
};

size_t CBufferDevice::Skip(size_t iMaxSize) {
  size_t iLastPos = _iPos;

  // Don't go past the size
  _iPos = dreamy::math::Min(_iPos + iMaxSize, Size());

  // Results in less than iMaxSize if limited by size
  return _iPos - iLastPos;
};

size_t CBufferDevice::Read(c8 *pData, size_t iMaxSize) {
  if (pData == nullptr || _pData == nullptr || _pData->IsNull()) {
    return NULL_POS;
  }

  if (AtEnd()) return 0;

  size_t iExpectedPos = _iPos + iMaxSize;

  // Past the limit
  if (iExpectedPos > Size()) {
    size_t iNotEnough = iExpectedPos - Size();
    iMaxSize -= iNotEnough;
  }

  memcpy(pData, &_pData->ConstData()[_iPos], iMaxSize);
  _iPos += iMaxSize;

  return iMaxSize;
};

size_t CBufferDevice::Peek(c8 *pData, size_t iMaxSize) {
  size_t iCurrentPos = Pos();
  size_t iLength = Read(pData, iMaxSize);
  Seek(iCurrentPos);

  return iLength;
};

size_t CBufferDevice::Write(const c8 *pData, size_t iMaxSize) {
  if (pData == nullptr || _pData == nullptr || !IsWritable()) {
    return NULL_POS;
  }

  if (_iPos + iMaxSize >= Size()) {
    _pData->Resize(Size() + iMaxSize);
  }

  memcpy(&_pData->Data()[_iPos], pData, iMaxSize);
  _iPos += iMaxSize;

  return iMaxSize;
};

void CBufferDevice::SetBuffer(CByteArray *pData) {
  if (IsOpen()) return;
  _pData = pData;
};

const c8 *CBufferDevice::GetBuffer(void) const {
  D_ASSERT(_pData != nullptr);
  return _pData->ConstData();
};

};
