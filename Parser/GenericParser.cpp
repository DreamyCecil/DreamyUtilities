//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#include "GenericParser.hpp"

namespace dreamy {

GenericParser::GenericParser(const CString &strSet) : str(strSet), pchCur(&strSet[0]), pchNext(&strSet[1]),
  iLineCur(0), iLineBeg(0), pos(0, 0, 0, 0)
{
};

void GenericParser::Start(void) {
  // Starting position
  pos.iFirst = pos.iLast;

  SetToCurrent();

  // Current line and character position (relative to the line)
  SetPosition(pos.iLast);
};

bool GenericParser::AtEnd(void) {
  return pos.iLast >= (u32)str.length();
};

void GenericParser::SetToCurrent(void) {
  pchCur = &str[pos.iLast];
  pchNext = pchCur + 1;
};

void GenericParser::Advance(u32 iOffset) {
  pos.iLast += iOffset;
};

bool GenericParser::CanParse(void) {
  // Already at the end
  if (AtEnd()) return false;

  // Parse a new character
  Start();
  Advance(1);

  return true;
};

void GenericParser::CountLine(void) {
  ++iLineCur;
  iLineBeg = pos.iLast;
};

void GenericParser::SetPosition(u32 iPos) {
  pos.FormatPos(iPos, iLineCur, iLineBeg);
};

CString GenericParser::ExtractString(u32 iBeginOffset) {
  iBeginOffset += pos.iFirst;
  return str.substr(iBeginOffset, pos.iLast - iBeginOffset);
};

void GenericParser::AddEOF(CTokenList &aTokens) {
  const u32 iEndPos = (u32)str.length();

  pos = CTokenPos(iEndPos, iEndPos, -1, -1);
  SetPosition(pos.iLast);

  AddToken(aTokens, CParserToken::TKN_EOF, pos);
};

}; // namespace dreamy
