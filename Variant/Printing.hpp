//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_VARIANT_PRINTING_H
#define _DREAMYUTILITIES_INCL_VARIANT_PRINTING_H

#include "../Base/Base.hpp"

#include "Variant.hpp"
#include "PrintOptions.hpp"

#include "../IO/StringStream.hpp"

namespace dreamy
{
  // Define method for printing a variant with specific function arguments
  #define DEFINE_VARIANT_PRINT(FuncName, ArgVal, ArgStrm, ArgOpts, ArgUndefined) \
    void FuncName(const CVariant &ArgVal, CStringStream &ArgStrm, const ValPrintOpts &ArgOpts, const c8 *ArgUndefined)

  // Define global method for printing out the variant
  #define VARIANT_PRINT_METHOD(TypeName) \
    DEFINE_VARIANT_PRINT(Print##TypeName, val, strm, opts, strUndefined) { \
      strm << val.To##TypeName(); \
    }

  // Define global method for printing out the variant with extras
  #define VARIANT_PRINT_METHOD_CUSTOM(TypeName, Printout) \
    DEFINE_VARIANT_PRINT(Print##TypeName, val, strm, opts, strUndefined) { \
      strm << Printout; \
    }

  // Define global dummy method for printing out the variant
  #define VARIANT_PRINT_METHOD_DUMMY(TypeName) \
    DEFINE_VARIANT_PRINT(Print##TypeName, val, strm, opts, strUndefined) { \
      strm << strUndefined; \
    }

  // Define global method for printing out the type array variant
  #define VARIANT_PRINT_METHOD_ARRAY(TypeName, ArrayPrefix) \
    DEFINE_VARIANT_PRINT(Print##TypeName, val, strm, opts, strUndefined) { \
      /* Add array prefix and convert to array of any type */ \
      strm << ArrayPrefix; \
      CValArray aPrint; \
      ToAnyArray(aPrint, val.To##TypeName()); \
      /* Print variant array */ \
      CVariant valArray; \
      valArray.FromArray(aPrint); \
      PrintArray(valArray, strm, opts, strUndefined); \
    }

  #if defined(_DREAMY_INDENT_WITH_SPACES) && _DREAMY_INDENT_WITH_SPACES >= 0
    // Indent values in the formatted printout with a specific amount of spaces
    #define VARIANT_PRINT_INDENT(Level) Str_t((Level) * _DREAMY_INDENT_WITH_SPACES, ' ')
  #else
    // Indent values in the formatted printout with tabs
    #define VARIANT_PRINT_INDENT(Level) Str_t((Level), '\t')
  #endif

  // Define printing methods
  typedef DEFINE_VARIANT_PRINT((*CVariantPrintFunc), val, strm, opts, strUndefined);

  // Variant printing declaration
  __forceinline DEFINE_VARIANT_PRINT(PrintVariant, val, strm, opts, strUndefined = "undefined");

  // Simple types
  inline VARIANT_PRINT_METHOD_DUMMY(Invalid); // Print undefined value
  inline VARIANT_PRINT_METHOD(F32); // Print single precision real number
  inline VARIANT_PRINT_METHOD(F64); // Print double precision real number
  inline VARIANT_PRINT_METHOD_CUSTOM(Bit, (val.ToBit() ? "true" : "false")); // Print bool state
  inline VARIANT_PRINT_METHOD(U8); // Print unsigned byte
  inline VARIANT_PRINT_METHOD(S8); // Print signed byte
  inline VARIANT_PRINT_METHOD(U16); // Print unsigned word
  inline VARIANT_PRINT_METHOD(S16); // Print signed word
  inline VARIANT_PRINT_METHOD(U32); // Print unsigned integer
  inline VARIANT_PRINT_METHOD(S32); // Print signed integer
  inline VARIANT_PRINT_METHOD(U64); // Print unsigned long integer
  inline VARIANT_PRINT_METHOD(S64); // Print signed long integer
  inline VARIANT_PRINT_METHOD_CUSTOM(String, '"' << val.ToString() << '"'); // Print string surrounded with quotes

  // Complex types

  // Print JSON array
  inline DEFINE_VARIANT_PRINT(PrintArray, val, strm, opts, strUndefined) {
    // No entries
    if (val.ToArray().size() == 0) {
      strm << "[]";
      return;
    }

    // Open the array
    strm << '[';

    // Character to print after individual elements (newline for the formatted type)
    c8 strNext[2] = "\n";

    // If inline
    if (opts.IsInline()) {
      // Compact mode (nothing after elements)
      strNext[0] = (opts.aiArgs[0] != 0) ? '\0' : ' ';
    }

    strm << strNext;

    // Print entries in order
    const CValArray &aArray = val.ToArray();
    CValArray::const_iterator it;

    for (it = aArray.begin(); it != aArray.end(); ++it) {
      // Separate values
      if (it != aArray.begin()) {
        strm << ',';
        strm << strNext;
      }

      ValPrintOpts optsNext(opts);

      // Indent values in the formatted printout
      if (opts.IsFormatted()) {
        optsNext.aiArgs[0] += 1;
        strm << VARIANT_PRINT_INDENT(optsNext.aiArgs[0]);
      }

      // <value>, <value>...
      PrintVariant(*it, strm, optsNext, strUndefined);
    }

    strm << strNext;

    // Original indentation before closing the array
    if (opts.IsFormatted()) {
      strm << VARIANT_PRINT_INDENT(opts.aiArgs[0]);
    }

    // Close the array
    strm << ']';
  };

  // Print array of any type inline
  inline void PrintInlineArray(const CValArray &aVariants, CStringStream &strm, const ValPrintOpts &opts, const c8 *strUndefined) {
    CVariant valArray;
    valArray.FromArray(aVariants);

    ValPrintOpts optsNext(ValPrintOpts::E_INLINE, false);

    // Copy inline's compact option
    if (opts.IsInline()) {
      optsNext.aiArgs[0] = opts.aiArgs[0];
    }

    PrintArray(valArray, strm, optsNext, strUndefined);
  };

  // Type arrays
  inline VARIANT_PRINT_METHOD_ARRAY(BitArray,  "ab"); // Print bits       ('b'ool)
  inline VARIANT_PRINT_METHOD_ARRAY(ByteArray, "ac"); // Print bytes      ('c'har)
  inline VARIANT_PRINT_METHOD_ARRAY(IntArray,  "ai"); // Print integers   ('i'nt)
  inline VARIANT_PRINT_METHOD_ARRAY(NumArray,  "af"); // Print numbers    ('f'loat)
  inline VARIANT_PRINT_METHOD_ARRAY(StrArray,  "as"); // Print strings    ('s'tring)
  inline VARIANT_PRINT_METHOD_ARRAY(Vec2Array, "av"); // Print 2D vectors ('v'ector)
  inline VARIANT_PRINT_METHOD_ARRAY(Vec3Array, "av"); // Print 3D vectors ('v'ector)

  // Print JSON object
  inline DEFINE_VARIANT_PRINT(PrintObject, val, strm, opts, strUndefined) {
    // No entries
    if (val.ToObject().size() == 0) {
      strm << "{}";
      return;
    }

    // Open the object
    strm << '{';

    // Character to print after individual elements (newline for the formatted type)
    c8 strNext[2] = "\n";

    // If inline
    if (opts.IsInline()) {
      // Compact mode (nothing after elements)
      strNext[0] = (opts.aiArgs[0] != 0) ? '\0' : ' ';
    }

    strm << strNext;

    // Print entries in order
    const CValObject &oObject = val.ToObject();
    CValObject::const_iterator it;

    for (it = oObject.begin(); it != oObject.end(); ++it) {
      // Separate values
      if (it != oObject.begin()) {
        strm << ',';
        strm << strNext;
      }

      ValPrintOpts optsNext(opts);

      // Print key name
      if (opts.IsInline()) {
        strm << '"' << it->first.GetString() << '"' << (opts.aiArgs[0] != 0 ? ":" : " : ");

      } else {
        // Indent values in the formatted printout
        optsNext.aiArgs[0] += 1;
        strm << VARIANT_PRINT_INDENT(optsNext.aiArgs[0]) << '"' << it->first.GetString() << "\" : ";
      }

      // "key" : <value>, "key" : <value>...
      PrintVariant(it->second, strm, optsNext, strUndefined);
    }

    strm << strNext;

    // Original indentation before closing the object
    if (opts.IsFormatted()) {
      strm << VARIANT_PRINT_INDENT(opts.aiArgs[0]);
    }

    // Close the object
    strm << '}';
  };

  // Print pointer
  inline DEFINE_VARIANT_PRINT(PrintPtr, val, strm, opts, strUndefined) {
    // Empty pointer
    if (val.ToPtr() == nullptr) {
      strm << "->null";
      return;
    }

    // Indicate that it points to some value inline
    if (opts.IsInline()) {
      strm << "->value";
      return;
    }

    // Print value from the pointer with no further recursion
    strm << "->";
    PrintVariant(*val.ToPtr(), strm, ValPrintOpts(ValPrintOpts::E_INLINE, true), strUndefined);
  };

  // Print 2D vector
  inline DEFINE_VARIANT_PRINT(PrintVec2, val, strm, opts, strUndefined) {
    strm << 'v'; // Prefix

    CValArray af(2);
    af[0].FromF64(val.ToVec2()[0]);
    af[1].FromF64(val.ToVec2()[1]);

    PrintInlineArray(af, strm, opts, strUndefined);
  };

  // Print 3D vector
  inline DEFINE_VARIANT_PRINT(PrintVec3, val, strm, opts, strUndefined) {
    strm << 'v'; // Prefix

    CValArray af(3);
    af[0].FromF64(val.ToVec3()[0]);
    af[1].FromF64(val.ToVec3()[1]);
    af[2].FromF64(val.ToVec3()[2]);

    PrintInlineArray(af, strm, opts, strUndefined);
  };

  // Print 2D matrix
  inline DEFINE_VARIANT_PRINT(PrintMat2, val, strm, opts, strUndefined) {
    strm << 'm'; // Prefix

    CValArray av(3);
    av[0].FromVec2(val.ToMat2()[0]);
    av[1].FromVec2(val.ToMat2()[1]);

    CVariant valArray;
    valArray.FromArray(av);

    PrintArray(valArray, strm, opts, strUndefined);
  };

  // Print 3D matrix
  inline DEFINE_VARIANT_PRINT(PrintMat3, val, strm, opts, strUndefined) {
    strm << 'm'; // Prefix

    CValArray av(3);
    av[0].FromVec3(val.ToMat3()[0]);
    av[1].FromVec3(val.ToMat3()[1]);
    av[2].FromVec3(val.ToMat3()[2]);

    CVariant valArray;
    valArray.FromArray(av);

    PrintArray(valArray, strm, opts, strUndefined);
  };

  // Printing methods per type
  static struct VariantPrinter {
    std::vector<CVariantPrintFunc> aMethods;

    // Constructor with main comparators
    VariantPrinter(void) {
      aMethods.resize(CVariant::VAL_LAST);

      // Simple types
      aMethods[CVariant::VAL_INVALID] = &PrintInvalid;
      aMethods[CVariant::VAL_F32] = &PrintF32;
      aMethods[CVariant::VAL_F64] = &PrintF64;
      aMethods[CVariant::VAL_BIT] = &PrintBit;
      aMethods[CVariant::VAL_U8]  = &PrintU8;
      aMethods[CVariant::VAL_S8]  = &PrintS8;
      aMethods[CVariant::VAL_U16] = &PrintU16;
      aMethods[CVariant::VAL_S16] = &PrintS16;
      aMethods[CVariant::VAL_U32] = &PrintU32;
      aMethods[CVariant::VAL_S32] = &PrintS32;
      aMethods[CVariant::VAL_U64] = &PrintU64;
      aMethods[CVariant::VAL_S64] = &PrintS64;
      aMethods[CVariant::VAL_STR] = &PrintString;

      // Complex types
      aMethods[CVariant::VAL_OBJ]  = &PrintObject;
      aMethods[CVariant::VAL_PTR]  = &PrintPtr;
      aMethods[CVariant::VAL_VEC2] = &PrintVec2;
      aMethods[CVariant::VAL_VEC3] = &PrintVec3;
      aMethods[CVariant::VAL_MAT2] = &PrintMat2;
      aMethods[CVariant::VAL_MAT3] = &PrintMat3;
      aMethods[CVariant::VAL_ARR]  = &PrintArray;

      // Type arrays
      aMethods[CVariant::VAL_ARR_BIT]  = &PrintBitArray;
      aMethods[CVariant::VAL_ARR_U8]   = &PrintByteArray;
      aMethods[CVariant::VAL_ARR_S64]  = &PrintIntArray;
      aMethods[CVariant::VAL_ARR_F64]  = &PrintNumArray;
      aMethods[CVariant::VAL_ARR_STR]  = &PrintStrArray;
      aMethods[CVariant::VAL_ARR_VEC2] = &PrintVec2Array;
      aMethods[CVariant::VAL_ARR_VEC3] = &PrintVec3Array;
    };
  } _variantPrinter;

  // Print variant value
  inline DEFINE_VARIANT_PRINT(PrintVariant, val, strm, opts, strUndefined) {
    CVariant::EType eType = val.GetType();
    _variantPrinter.aMethods[eType](val, strm, opts, strUndefined);
  };

};

#endif // (Dreamy Utilities Include Guard)
