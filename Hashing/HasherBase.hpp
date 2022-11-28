//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_HASHERBASE_H
#define _DREAMYUTILITIES_INCL_HASHERBASE_H

#include "../Base/Base.hpp"

namespace dreamy
{
  class IHasher {
    protected:
      // Hash an array of bytes
      virtual void Hash(const c8 *pData, size_t iSize) = 0;
  };

};

#endif // (Dreamy Utilities Include Guard)
