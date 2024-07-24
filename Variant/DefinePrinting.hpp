//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_VARIANT_PRINTING_H
#define _DREAMYUTILITIES_INCL_VARIANT_PRINTING_H

#include "../Base/Base.hpp"

#include "../Variant/Variant.hpp"
#include "../Variant/DeclarePrinting.hpp"

#include "../IO/StringStream.hpp"

namespace dreamy {

// Define method for printing a variant into a stream with specific function arguments
#define VARIANT_DEFINE_PRINT(FuncName, ArgVal, ArgStrm, ArgOpts, ArgUndefined) \
  void FuncName(const CVariant &ArgVal, CStringStream &ArgStrm, const ValPrintOpts &ArgOpts, const c8 *ArgUndefined)

// Define global method for printing out the variant
#define VARIANT_PRINT_METHOD(TypeName) \
  VARIANT_DEFINE_PRINT(Print##TypeName, val, strm, opts, strUndefined) { \
    strm << val.To##TypeName(); \
  }

// Define global method for printing out the variant with extras
#define VARIANT_PRINT_METHOD_CUSTOM(TypeName, Printout) \
  VARIANT_DEFINE_PRINT(Print##TypeName, val, strm, opts, strUndefined) { \
    strm << Printout; \
  }

// Define global method for printing out the type array variant
#define VARIANT_PRINT_METHOD_ARRAY(TypeName, ArrayPrefix) \
  VARIANT_DEFINE_PRINT(Print##TypeName, val, strm, opts, strUndefined) { \
    /* Add array prefix and convert to array of any type */ \
    strm << ArrayPrefix; \
    CValArray aPrint; \
    ToAnyArray(aPrint, val.To##TypeName()); \
    PrintArray(aPrint, strm, opts, strUndefined); \
  }

#if defined(_DREAMY_INDENT_WITH_SPACES) && _DREAMY_INDENT_WITH_SPACES >= 0
  // Indent values in the formatted printout with a specific amount of spaces
  #define VARIANT_PRINT_INDENT(Level) CString((Level) * _DREAMY_INDENT_WITH_SPACES, ' ')
#else
  // Indent values in the formatted printout with tabs
  #define VARIANT_PRINT_INDENT(Level) CString((Level), '\t')
#endif

// Simple types
VARIANT_PRINT_METHOD_CUSTOM(Invalid, strUndefined); // Print undefined value
VARIANT_PRINT_METHOD_CUSTOM(Bool, (val.ToBool() ? "true" : "false"));
VARIANT_PRINT_METHOD(Float);
VARIANT_PRINT_METHOD(Int);

// Print string optionally surrounded with quotes
VARIANT_DEFINE_PRINT(PrintString, val, strm, opts, strUndefined) {
  if (opts.eType == ValPrintOpts::E_INLINE && opts.aiArgs[1]) {
    strm << val.ToString();
  } else {
    strm << '"' << val.ToString() << '"';
  }
};

// Complex types

// Print JSON array
VARIANT_DEFINE_PRINT(PrintArray, val, strm, opts, strUndefined) {
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
    it->Print(strm, optsNext, strUndefined);
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
  ValPrintOpts optsNext(ValPrintOpts::E_INLINE, false);

  // Copy inline's compact option
  if (opts.IsInline()) {
    optsNext.aiArgs[0] = opts.aiArgs[0];
  }

  PrintArray(aVariants, strm, optsNext, strUndefined);
};

// Type arrays
VARIANT_PRINT_METHOD_ARRAY(BoolArray,  "ab"); // Print bits       ('b'ool)
VARIANT_PRINT_METHOD_ARRAY(ByteArray,  "ac"); // Print bytes      ('c'har)
VARIANT_PRINT_METHOD_ARRAY(IntArray,   "ai"); // Print integers   ('i'nt)
VARIANT_PRINT_METHOD_ARRAY(FloatArray, "af"); // Print numbers    ('f'loat)
VARIANT_PRINT_METHOD_ARRAY(StrArray,   "as"); // Print strings    ('s'tring)
VARIANT_PRINT_METHOD_ARRAY(Vec2Array,  "av"); // Print 2D vectors ('v'ector)
VARIANT_PRINT_METHOD_ARRAY(Vec3Array,  "av"); // Print 3D vectors ('v'ector)

// Print JSON object
VARIANT_DEFINE_PRINT(PrintObject, val, strm, opts, strUndefined) {
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
    it->second.Print(strm, optsNext, strUndefined);
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
VARIANT_DEFINE_PRINT(PrintPtr, val, strm, opts, strUndefined) {
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
  val.ToPtr()->Print(strm, ValPrintOpts(ValPrintOpts::E_INLINE, true), strUndefined);
};

// Print 2D vector
VARIANT_DEFINE_PRINT(PrintVec2, val, strm, opts, strUndefined) {
  strm << 'v'; // Prefix

  CValArray af(2);
  af[0].FromFloat(val.ToVec2()[0]);
  af[1].FromFloat(val.ToVec2()[1]);

  PrintInlineArray(af, strm, opts, strUndefined);
};

// Print 3D vector
VARIANT_DEFINE_PRINT(PrintVec3, val, strm, opts, strUndefined) {
  strm << 'v'; // Prefix

  CValArray af(3);
  af[0].FromFloat(val.ToVec3()[0]);
  af[1].FromFloat(val.ToVec3()[1]);
  af[2].FromFloat(val.ToVec3()[2]);

  PrintInlineArray(af, strm, opts, strUndefined);
};

// Print 2D matrix
VARIANT_DEFINE_PRINT(PrintMat2, val, strm, opts, strUndefined) {
  strm << 'm'; // Prefix

  CValArray av(3);
  av[0].FromVec2(val.ToMat2()[0]);
  av[1].FromVec2(val.ToMat2()[1]);

  PrintArray(av, strm, opts, strUndefined);
};

// Print 3D matrix
VARIANT_DEFINE_PRINT(PrintMat3, val, strm, opts, strUndefined) {
  strm << 'm'; // Prefix

  CValArray av(3);
  av[0].FromVec3(val.ToMat3()[0]);
  av[1].FromVec3(val.ToMat3()[1]);
  av[2].FromVec3(val.ToMat3()[2]);

  PrintArray(av, strm, opts, strUndefined);
};

};

#endif // (Dreamy Utilities Include Guard)
