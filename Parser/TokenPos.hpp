//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_PARSER_BASE_H
#define _DREAMYUTILITIES_INCL_PARSER_BASE_H

#include "../Base/Base.hpp"

#include "../Math/Algorithm.hpp"

namespace dreamy {

// Maximum amount of characters in a line
#define CHARS_PER_LINE s32(32000)

// Token position within the string
class CTokenPos {

public:
  u32 iFirst; // First character index
  u32 iLast; // Last character index

  u32 iLine; // Line index (from 0)
  u32 iCol; // Column index (from 0)

public:
  // Default constructor
  CTokenPos() : iFirst(-1), iLast(-1), iLine(-1), iCol(-1)
  {
  };

  // Constructor with positions
  CTokenPos(u32 iSetFirst, u32 iSetLast, u32 iSetLine, u32 iSetCol) :
    iFirst(iSetFirst), iLast(iSetLast), iLine(iSetLine), iCol(iSetCol)
  {
  };

  // Copy constructor
  CTokenPos(const CTokenPos &posOther) {
    operator=(posOther);
  };

  // Get token length
  inline u32 Length(void) const {
    return iLast - iFirst;
  };

  // Shift token position (doesn't affect formatted position)
  inline void ShiftPos(s32 iOffset) {
    iFirst += iOffset;
    iLast += iOffset;
  };

  // Format token position relative to the line
  inline void FormatPos(u32 iCurrentPos, u32 iCurrentLine, u32 iLineBeginning) {
    iLine = iCurrentLine;
    iCol = iCurrentPos - iLineBeginning;
  };

  // Assignment
  inline CTokenPos &operator=(const CTokenPos &posOther) {
    iFirst = posOther.iFirst;
    iLast = posOther.iLast;
    iLine = posOther.iLine;
    iCol = posOther.iCol;

    return *this;
  };

  // Compare positions
  inline bool operator==(const CTokenPos &posOther) const {
    return (iFirst == posOther.iFirst
          && iLast == posOther.iLast
          && iLine == posOther.iLine
          && iCol == posOther.iCol);
  };

  // Retrieve token line and column
  inline void GetPos(u32 &iGetLine, u32 &iGetCol) const {
    // Start from 1
    iGetLine = iLine + 1;
    iGetCol = iCol + 1;
  };
};

};

// Undefine min function
#undef MIN_FUNC

#endif // (Dreamy Utilities Include Guard)
