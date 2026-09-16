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
