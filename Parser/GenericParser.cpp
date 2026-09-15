//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

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
