//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_PARSER_EXCEPTION_H
#define _DREAMYUTILITIES_INCL_PARSER_EXCEPTION_H

#include "../Base/Base.hpp"

#include "../Parser/TokenPos.hpp"

namespace dreamy
{
  // Exception that a tokenizer may throw
  class CTokenException : public CMessageException {
    protected:
      CTokenPos _pos;

    public:
      // Default constructor
      CTokenException(const CTokenPos &pos, const c8 *strError = "") : _pos(pos)
      {
        u32 iLine, iCol;
        _pos.GetPos(iLine, iCol);

        PrintF("%s at line %u, col %u", strError, iLine, iCol);
      };

      // Get error position
      const CTokenPos GetPos(void) const {
        return _pos;
      };

      // Quick function for throwing token exceptions
      static void Throw(const CTokenPos &pos, const c8 *strFormat, ...) {
        CTokenException ex(pos);
        DREAMY_PRINTF_INLINE(ex._message, strFormat);

        throw ex;
      };
  };

};

#endif // (Dreamy Utilities Include Guard)
