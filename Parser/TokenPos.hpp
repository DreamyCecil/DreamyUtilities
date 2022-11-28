//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_PARSER_BASE_H
#define _DREAMYUTILITIES_INCL_PARSER_BASE_H

#include "../Base/Base.hpp"

#include "../Math/Algorithm.hpp"

namespace dreamy
{
  // Maximum amount of characters in a line
  #define CHARS_PER_LINE s32(32000)

  // Token position within the string
  class CTokenPos {
    public:
      u32 iFirst; // First character index
      u32 iLast; // Last character index
      u32 iFormatted; // Line and column indices combined

    public:
      // Default constructor
      CTokenPos(void) : iFirst(-1), iLast(-1), iFormatted(-1)
      {
      };

      // Constructor with positions
      CTokenPos(const u32 iSetFirst, const u32 iSetLast, const u32 iSetFormatted) :
        iFirst(iSetFirst), iLast(iSetLast), iFormatted(iSetFormatted)
      {
      };

      // Copy constructor
      CTokenPos(const CTokenPos &posOther) {
        operator=(posOther);
      };

      // Get token length
      inline const u32 Length(void) const {
        return iLast - iFirst;
      };

      // Shift token position (doesn't affect formatted position)
      inline void ShiftPos(const s32 iOffset) {
        iFirst += iOffset;
        iLast += iOffset;
      };

      // Format token position relative to the line
      inline void FormatPos(const u32 iCurrentPos, const u32 iCurrentLine, const u32 iLineBeginning) {
        iFormatted = iCurrentLine * CHARS_PER_LINE + math::Min(iCurrentPos - iLineBeginning, u32(CHARS_PER_LINE - 1));
      };

      // Assignment
      inline CTokenPos &operator=(const CTokenPos &posOther) {
        iFirst = posOther.iFirst;
        iLast = posOther.iLast;
        iFormatted = posOther.iFormatted;

        return *this;
      };

      // Compare positions
      inline bool operator==(const CTokenPos &posOther) const {
        return (iFirst == posOther.iFirst
             && iLast == posOther.iLast
             && iFormatted == posOther.iFormatted);
      };

      // Retrieve token line and column
      inline void GetPos(u32 &iLine, u32 &iCol) const {
        iCol = iFormatted % CHARS_PER_LINE; // Character in a line
        iLine = (iFormatted - iCol) / CHARS_PER_LINE; // Line
        ++iCol; // Start from 1
      };
  };

};

// Undefine min function
#undef MIN_FUNC

#endif // (Dreamy Utilities Include Guard)
