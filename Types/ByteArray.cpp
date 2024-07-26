//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#include "ByteArray.hpp"

namespace dreamy {

CByteArray::CByteArray() : _pBuffer(nullptr), _iSize(0)
{
};

CByteArray::CByteArray(const CByteArray &baOther) : _pBuffer(nullptr), _iSize(0)
{
  Copy(baOther);
};

CByteArray::CByteArray(const c8 *pData, size_t iSize) : _pBuffer(nullptr), _iSize(0)
{
  if (pData == nullptr) return;

  // Calculate length of the array
  if (iSize == NULL_POS) {
    iSize = strlen(pData);
  }

  if (iSize != 0) {
    Resize(iSize);
    memcpy(_pBuffer, pData, iSize);
  }
};

CByteArray::CByteArray(c8 chByte, size_t iSize) : _pBuffer(nullptr), _iSize(0)
{
  if (iSize != 0) {
    Resize(iSize);
    memset(_pBuffer, chByte, iSize);
  }
};

CByteArray::~CByteArray() {
  Clear();
};

// Copy data from another byte array
void CByteArray::Copy(const CByteArray &baOther) {
  Clear();

  if (!baOther.IsNull() && baOther.Size() != 0) {
    Resize(baOther.Size());
    memcpy(_pBuffer, baOther.ConstData(), baOther.Size());
  }
};

CByteArray &CByteArray::Insert(size_t iPos, const c8 *pData, size_t iSize) {
  // Nothing to add
  if (pData == nullptr) {
    return *this;
  }

  // Calculate length of the array
  if (iSize == NULL_POS) {
    iSize = strlen(pData);
  }

  if (iPos > Size()) {
    iPos = Size();
  }

  // Allocate new buffer
  size_t iNewSize = Size() + iSize;
  c8 *pNewData = new c8[iNewSize + 1]; // Size + terminator
  pNewData[iNewSize] = '\0';

  // Copy first part of the source array
  if (iPos != 0) {
    memcpy(&pNewData[0], Data(), iPos);
  }

  memcpy(&pNewData[iPos], pData, iSize); // Copy data from another array
  memcpy(&pNewData[iPos + iSize], &Data()[iPos], Size() - iPos); // Copy second part of the source array

  // Delete old buffer
  Clear();

  _pBuffer = pNewData;
  _iSize = iNewSize;

  return *this;
};

CByteArray &CByteArray::Insert(size_t iPos, size_t iCount, c8 chByte) {
  // Nothing to add
  if (iCount == 0) {
    return *this;
  }

  if (iPos > Size()) {
    iPos = Size();
  }

  // Allocate new buffer
  size_t iNewSize = Size() + iCount;
  c8 *pNewData = new c8[iNewSize + 1]; // Size + terminator
  pNewData[iNewSize] = '\0';

  // Copy first part of source array
  if (iPos != 0) {
    memcpy(&pNewData[0], Data(), iPos);
  }

  memset(&pNewData[iPos], chByte, iCount); // Fill with bytes
  memcpy(&pNewData[iPos + iCount], &Data()[iPos], Size() - iPos); // Copy second part of the source array

  // Delete old buffer
  Clear();

  _pBuffer = pNewData;
  _iSize = iNewSize;

  return *this;
};

CByteArray &CByteArray::Remove(size_t iPos, size_t iSize) {
  if (IsNull() || iSize == 0 || iPos >= Size()) {
    return *this;
  }

  size_t iMaxRemoval = Size() - iPos;

  if (iSize > iMaxRemoval) {
    iSize = iMaxRemoval;
  }

  size_t iNewSize = Size() - iSize;
  size_t iRightSize = Size() - iSize - iPos;

  if (iRightSize != 0) {
    memcpy(&Data()[iPos], &Data()[iPos + iSize], iRightSize);
  }

  Resize(iNewSize);
  return *this;
};

void CByteArray::Resize(size_t iNewSize) {
  if (iNewSize == 0) {
    Clear();
    return;
  }

  c8 *pNewData = new c8[iNewSize + 1]; // Size + terminator
  pNewData[iNewSize] = '\0';

  if (Data()) {
    // Copy old data if enough space
    if (iNewSize > Size()) {
      if (_iSize != 0) {
        memcpy(&pNewData[0], _pBuffer, _iSize);
      }

    // Copy reduced data
    } else {
      if (iNewSize != 0) {
        memcpy(&pNewData[0], _pBuffer, iNewSize);
      }
    }

    Clear(); // Delete old data
  }

  // Set new buffer
  _pBuffer = pNewData;
  _iSize = iNewSize;
};

void CByteArray::Chop(size_t iSize) {
  if (iSize > Size()) {
    iSize = Size();
  }

  Resize(Size() - iSize);
};

void CByteArray::Clear(void) {
  if (_pBuffer != nullptr) {
    delete[] _pBuffer;

    _pBuffer = nullptr;
    _iSize = 0;
  }
};

c8 CByteArray::At(size_t i) const {
  D_ASSERT(_pBuffer != nullptr);
  D_ASSERT(i < _iSize);

  return _pBuffer[i];
};

CByteArray &CByteArray::Fill(const c8 chByte, size_t iSize) {
  if (Size() == 0 || IsNull()) {
    return *this;
  }

  if (iSize > Size()) {
    iSize = Size();
  }

  memset(_pBuffer, chByte, iSize);

  return *this;
};

CByteArray &CByteArray::operator=(const CByteArray &baOther) {
  Clear();

  if (baOther.Size() != 0) {
    Resize(baOther.Size());
    memcpy(Data(), baOther.ConstData(), baOther.Size());
  }

  return *this;
};

bool CByteArray::operator==(const CByteArray &baOther) const {
  // Different sizes 
  if (Size() != baOther.Size()) {
    return false;
  }

  // Both are empty
  if (IsNull() && baOther.IsNull()) {
    return true;
  }

  return memcmp(ConstData(), baOther.ConstData(), Size()) == 0;
};

bool CByteArray::operator==(const c8 *strOther) const {
  // String is null and empty array
  if (IsNull() && strOther == nullptr) {
    return true;
  }

  // String is null but array is not empty
  if (strOther == nullptr) {
    return false;
  }

  size_t iSize = strlen(strOther);

  // Different sizes
  if (Size() != iSize) {
    return false;
  }

  return memcmp(ConstData(), strOther, Size()) == 0;
};

void CByteArray::Swap(CByteArray &baOther) {
  size_t iTempSize = Size();
  c8 *pTempData = Data();

  _iSize = baOther.Size();
  _pBuffer = baOther.Data();

  baOther._iSize = iTempSize;
  baOther._pBuffer = pTempData;
};

bool CByteArray::StartsWith(const CByteArray &baOther) const {
  if (baOther.IsNull() || baOther.Size() > Size()) {
    return false;
  }

  return memcmp(ConstData(), baOther.ConstData(), baOther.Size()) == 0;
};

bool CByteArray::StartsWith(const c8 chByte) const {
  if (Size() == 0) {
    return false;
  }

  return Front() == chByte;
};

bool CByteArray::StartsWith(const c8 *str) const {
  if (str == nullptr) {
    return false;
  }

  size_t iSize = strlen(str);

  if (iSize > Size()) {
    return false;
  }

  return memcmp(ConstData(), str, iSize) == 0;
};

bool CByteArray::EndsWith(const CByteArray &baOther) const {
  if (baOther.IsNull() || baOther.Size() > Size()) {
    return false;
  }

  size_t iPos = Size() - baOther.Size();
  return memcmp(&ConstData()[iPos], baOther.ConstData(), baOther.Size()) == 0;
};

bool CByteArray::EndsWith(const c8 chByte) const {
  if (Size() == 0) {
    return false;
  }

  return Back() == chByte;
};

bool CByteArray::EndsWith(const c8 *str) const {
  if (str == nullptr) {
    return false;
  }

  size_t iSize = strlen(str);

  if (iSize > Size()) {
    return false;
  }

  size_t iPos = Size() - iSize;
  return memcmp(&ConstData()[iPos], str, iSize) == 0;
};

size_t CByteArray::IndexOf(const CByteArray &baOther, size_t iFrom) {
  if (IsNull() || baOther.IsNull() || iFrom >= Size() || baOther.Size() > Size()) {
    return NULL_POS;
  }

  for (size_t i = 0; i < Size(); ++i) {
    size_t iLeft = Size() - i;

    if (iLeft < baOther.Size()) {
      return NULL_POS;
    }

    if (memcmp(&ConstData()[i], baOther.ConstData(), baOther.Size()) == 0) {
      return i;
    }
  }

  return NULL_POS;
};

size_t CByteArray::IndexOf(c8 chByte, size_t iFrom) {
  if (IsNull() || iFrom >= Size()) {
    return NULL_POS;
  }

  for (size_t i = iFrom; i < Size(); ++i) {
    if (ConstData()[i] == chByte) {
      return i;
    }
  }

  return NULL_POS;
};

size_t CByteArray::IndexOf(const c8 *str, size_t iFrom) {
  if (IsNull() || str == nullptr || iFrom >= Size()) {
    return NULL_POS;
  }

  size_t iSize = strlen(str);

  if (iSize > Size()) {
    return NULL_POS;
  }

  for (size_t i = iFrom; i < Size(); ++i) {
    size_t iLeft = Size() - i;

    if (iLeft < iSize) {
      return NULL_POS;
    }

    if (memcmp(&ConstData()[i], str, iSize) == 0) {
      return i;
    }
  }

  return NULL_POS;
};

CByteArray CByteArray::ToHex(const c8 chDelimiter) const {
  static const c8 *strDigits = "0123456789ABCDEF";

  size_t iLength = chDelimiter == '\0' ? Size() * 2 : (Size() * 3 - 1);

  CByteArray baResult('0', iLength);

  size_t iElement = 0;

  for (size_t i = 0; i < Size(); ++i) {
    baResult.Data()[iElement++] = strDigits[(ConstData()[i] & 0xF0) >> 4];
    baResult.Data()[iElement++] = strDigits[ConstData()[i] & 0x0F];

    if (chDelimiter != '\0' && (i + 1) < Size()) {
      baResult.Data()[iElement++] = chDelimiter;
    }
  }

  return baResult;
};

};
