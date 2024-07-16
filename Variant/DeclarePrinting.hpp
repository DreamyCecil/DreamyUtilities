//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_VARIANT_PRINTOPTIONS_H
#define _DREAMYUTILITIES_INCL_VARIANT_PRINTOPTIONS_H

#include "../Base/Base.hpp"

#include "../IO/StringStream.hpp"

namespace dreamy {

// Structure that handles options for specific printout of variant types
struct ValPrintOpts {
  // Printout type
  enum EPrintType {
    // Single line (except when they contain newlines, e.g. strings)
    // [arg1 = compact : bool]
    // [arg2 = strings without quotes : bool]
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
    aiArgs[0] = optsOther.aiArgs[0];
    aiArgs[1] = optsOther.aiArgs[1];
    aiArgs[2] = optsOther.aiArgs[2];
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

// Declare method for printing a variant into a stream
#define VARIANT_DECLARE_PRINT(FuncName) \
  void FuncName(const class CVariant &val, CStringStream &strm, const ValPrintOpts &opts, const c8 *strUndefined)

// Printing method function type
typedef VARIANT_DECLARE_PRINT((*CVariantPrintFunc));

// Declare type-specific printing methods
inline VARIANT_DECLARE_PRINT(PrintInvalid);

inline VARIANT_DECLARE_PRINT(PrintBool);
inline VARIANT_DECLARE_PRINT(PrintFloat);
inline VARIANT_DECLARE_PRINT(PrintInt);
inline VARIANT_DECLARE_PRINT(PrintString);

inline VARIANT_DECLARE_PRINT(PrintObject);
inline VARIANT_DECLARE_PRINT(PrintPtr);

inline VARIANT_DECLARE_PRINT(PrintVec2);
inline VARIANT_DECLARE_PRINT(PrintVec3);
inline VARIANT_DECLARE_PRINT(PrintMat2);
inline VARIANT_DECLARE_PRINT(PrintMat3);

inline VARIANT_DECLARE_PRINT(PrintArray);
inline VARIANT_DECLARE_PRINT(PrintBoolArray);
inline VARIANT_DECLARE_PRINT(PrintByteArray);
inline VARIANT_DECLARE_PRINT(PrintIntArray);
inline VARIANT_DECLARE_PRINT(PrintFloatArray);
inline VARIANT_DECLARE_PRINT(PrintStrArray);
inline VARIANT_DECLARE_PRINT(PrintVec2Array);
inline VARIANT_DECLARE_PRINT(PrintVec3Array);

};

#endif // (Dreamy Utilities Include Guard)
