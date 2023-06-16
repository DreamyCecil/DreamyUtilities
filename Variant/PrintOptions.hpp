//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_VARIANT_PRINTOPTIONS_H
#define _DREAMYUTILITIES_INCL_VARIANT_PRINTOPTIONS_H

#include "../Base/Base.hpp"

namespace dreamy {

// Structure that handles options for specific printout of variant types
struct ValPrintOpts {
  // Printout type
  enum EPrintType {
    // Single line (except when they contain newlines, e.g. strings)
    // [arg1 = compact : bool]
    E_INLINE,

    // JSON-like blocks with automatic indentation
    // [arg1 = indentation level : s32]
    E_FORMATTED,
  } eType;

  s32 aiArgs[3]; // Printout arguments

  // Default constructor
  ValPrintOpts(const EPrintType eSetType = E_INLINE, s32 iArg1 = 0, s32 iArg2 = 0, s32 iArg3 = 0) :
    eType(eSetType)
  {
    aiArgs[0] = iArg1;
    aiArgs[1] = iArg2;
    aiArgs[2] = iArg3;
  };

  // Copy constructor
  ValPrintOpts(const ValPrintOpts &optsOther) : eType(optsOther.eType) {
    memcpy(aiArgs, optsOther.aiArgs, sizeof(aiArgs));
  };

  // Get printout type
  EPrintType GetType(void) const {
    return eType;
  };

  // Check if inline printout
  bool IsInline(void) const {
    return GetType() == E_INLINE;
  };

  // Check if formatted printout
  bool IsFormatted(void) const {
    return GetType() == E_FORMATTED;
  };
};

};

#endif // (Dreamy Utilities Include Guard)
