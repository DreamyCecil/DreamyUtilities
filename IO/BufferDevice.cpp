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

#include "BufferDevice.hpp"
#include "../Math/Algorithm.hpp"

NAMESPACE_DREAMY_OPEN

CBufferDevice::CBufferDevice() : m_pData(nullptr), m_iPos(0)
{
};

CBufferDevice::CBufferDevice(CByteArray *pByteArray) : m_pData(pByteArray), m_iPos(0)
{
};

bool CBufferDevice::Open(EOpenMode eOpenMode) {
  if (m_pData == nullptr) {
    return false;
  }

  m_eOpenMode = eOpenMode;
  m_iPos = 0;
  return IsOpen();
};

void CBufferDevice::Close(void) {
  m_eOpenMode = OM_UNOPEN;
  m_iPos = 0;
};

size_t CBufferDevice::Pos(void) const {
  return IsOpen() ? m_iPos : NULL_POS;
};

bool CBufferDevice::AtEnd(void) const {
  return Pos() >= Size();
};

size_t CBufferDevice::Size(void) const {
  return m_pData->Size();
};

bool CBufferDevice::Seek(size_t iOffset) {
  if (m_pData == nullptr || IsOpen()) {
    return false;
  }

  // Past the limit
  if (iOffset >= Size()) return false;

  // Set new position
  m_iPos = iOffset;
  return true;
};

size_t CBufferDevice::Skip(size_t iMaxSize) {
  size_t iLastPos = m_iPos;

  // Don't go past the size
  m_iPos = dreamy::math::Min(m_iPos + iMaxSize, Size());

  // Results in less than iMaxSize if limited by size
  return m_iPos - iLastPos;
};

size_t CBufferDevice::Read(c8 *pData, size_t iMaxSize) {
  if (pData == nullptr || m_pData == nullptr || m_pData->IsNull()) {
    return NULL_POS;
  }

  if (AtEnd()) return 0;

  size_t iExpectedPos = m_iPos + iMaxSize;

  // Past the limit
  if (iExpectedPos > Size()) {
    size_t iNotEnough = iExpectedPos - Size();
    iMaxSize -= iNotEnough;
  }

  memcpy(pData, &m_pData->ConstData()[m_iPos], iMaxSize);
  m_iPos += iMaxSize;

  return iMaxSize;
};

size_t CBufferDevice::Peek(c8 *pData, size_t iMaxSize) {
  size_t iCurrentPos = Pos();
  size_t iLength = Read(pData, iMaxSize);
  Seek(iCurrentPos);

  return iLength;
};

size_t CBufferDevice::Write(const c8 *pData, size_t iMaxSize) {
  if (pData == nullptr || m_pData == nullptr || !IsWritable()) {
    return NULL_POS;
  }

  if (m_iPos + iMaxSize >= Size()) {
    m_pData->Resize(Size() + iMaxSize);
  }

  memcpy(&m_pData->Data()[m_iPos], pData, iMaxSize);
  m_iPos += iMaxSize;

  return iMaxSize;
};

void CBufferDevice::SetBuffer(CByteArray *pData) {
  if (IsOpen()) return;
  m_pData = pData;
};

const c8 *CBufferDevice::GetBuffer(void) const {
  D_ASSERT(m_pData != nullptr);
  return m_pData->ConstData();
};

NAMESPACE_DREAMY_CLOSE
