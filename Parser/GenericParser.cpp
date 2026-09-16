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

#include "GenericParser.hpp"

namespace dreamy {

CGenericParser::CGenericParser(const CString &strSet) : m_str(strSet), m_pchCur(&strSet[0]), m_pchNext(&strSet[1]),
  m_iLineCur(1), m_iLineBeg(0), m_pos(0, 0, 1, 1)
{
};

void CGenericParser::Initialize(void) {
  m_iLineCur = 1;
  m_iLineBeg = 0;
  m_pos = CTokenPos(0, 0, 1, 1);
  StartToken();
};

bool CGenericParser::AtEnd(void) {
  return m_pos.iEnd >= (u32)m_str.length();
};

void CGenericParser::StartToken(void) {
  // Start new token from the current end position
  m_pos.iStart = m_pos.iEnd;
  SetCurrentChar();

  // Set current line and column
  UpdateTokenPlace(m_pos.iEnd);
};

void CGenericParser::SetCurrentChar(void) {
  m_pchCur = &m_str[m_pos.iEnd];
  m_pchNext = m_pchCur + 1;
};

void CGenericParser::Rewind(void) {
  m_pos.iEnd = m_pos.iStart;
  SetCurrentChar();
};

void CGenericParser::SetPos(u32 iFromTokenStart) {
  m_pos.iEnd = m_pos.iStart + iFromTokenStart;
  SetCurrentChar();
};

void CGenericParser::Advance(s32 iOffset) {
  m_pos.iEnd += iOffset;
  SetCurrentChar();
};

void CGenericParser::CountLine(void) {
  ++m_iLineCur;
  m_iLineBeg = m_pos.iEnd;
};

void CGenericParser::UpdateTokenPlace(u32 iPos) {
  m_pos.SetPlace(m_iLineCur, iPos - m_iLineBeg + 1);
};

CString CGenericParser::ExtractString(u32 iBeginOffset) {
  D_ASSERT(iBeginOffset <= m_pos.Length());
  iBeginOffset += m_pos.iStart;
  return m_str.substr(iBeginOffset, m_pos.iEnd - iBeginOffset);
};

void CGenericParser::AddEOF(CTokenList &aTokens) {
  const u32 iEndPos = (u32)m_str.length();

  m_pos.SetInvalid(iEndPos);
  UpdateTokenPlace(iEndPos);

  AddToken(aTokens, CParserToken::TKN_EOF, m_pos);
};

}; // namespace dreamy
