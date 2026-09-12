//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#include "SimpleHasher.hpp"

NAMESPACE_DREAMY_OPEN

CSimpleHasher::CSimpleHasher(u64 iSetFactor) : m_iResult(0), m_iFactor(iSetFactor)
{
};

void CSimpleHasher::Reset(void) {
  m_iResult = 0;
};

void CSimpleHasher::AddData(const c8 *pData, size_t iSize)
{
  for (size_t i = 0; i < iSize; ++i) {
    m_iResult = m_iFactor * m_iResult + pData[i];
  }
};

CByteArray CSimpleHasher::GetBytes(void) const
{
  return CByteArray(reinterpret_cast<const c8 *>(&m_iResult), sizeof(m_iResult));
};

u64 CSimpleHasher::GetResult(void) const {
  return m_iResult;
};

u64 CSimpleHasher::GetFactor(void) const {
  return m_iFactor;
};

void CSimpleHasher::SetFactor(u64 iSetFactor) {
  m_iFactor = iSetFactor;
};

u64 CSimpleHasher::operator()(const c8 *pData, size_t iSize) {
  Reset();
  AddData(pData, iSize);

  return m_iResult;
};

NAMESPACE_DREAMY_CLOSE
