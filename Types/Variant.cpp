//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#include "Variant.hpp"

namespace dreamy {

// Define global method for comparing variants
#define VARIANT_COMPARE_METHOD(TypeName) \
  inline bool Compare##TypeName(const CVariant &val1, const CVariant &val2) { \
    return val1.To##TypeName() == val2.To##TypeName(); \
  }

// Define global method for comparing variants using custom formula
#define VARIANT_COMPARE_METHOD_CUSTOM(TypeName, Comparison) \
  inline bool Compare##TypeName(const CVariant &val1, const CVariant &val2) { \
    return Comparison; \
  }

// Define comparison methods
typedef bool (*CVariantCompareFunc)(const CVariant &, const CVariant &);

VARIANT_COMPARE_METHOD_CUSTOM(Invalid, true);

VARIANT_COMPARE_METHOD(Bool);
VARIANT_COMPARE_METHOD(Float);
VARIANT_COMPARE_METHOD(Int);
VARIANT_COMPARE_METHOD(String);

VARIANT_COMPARE_METHOD(Object);
VARIANT_COMPARE_METHOD(Ptr);

VARIANT_COMPARE_METHOD(Vec2);
VARIANT_COMPARE_METHOD(Vec3);
VARIANT_COMPARE_METHOD_CUSTOM(Mat2, CompareMatrices(val1.ToMat2(), val2.ToMat2()));
VARIANT_COMPARE_METHOD_CUSTOM(Mat3, CompareMatrices(val1.ToMat3(), val2.ToMat3()));

VARIANT_COMPARE_METHOD(Array);
VARIANT_COMPARE_METHOD(BoolArray);
VARIANT_COMPARE_METHOD(ByteArray);
VARIANT_COMPARE_METHOD(IntArray);
VARIANT_COMPARE_METHOD(FloatArray);
VARIANT_COMPARE_METHOD(StrArray);
VARIANT_COMPARE_METHOD(Vec2Array);
VARIANT_COMPARE_METHOD(Vec3Array);

// Compare main types directly
bool CVariant::Compare(const CVariant &valOther) const {
  const EType eThis = GetType();
  const EType eOther = valOther.GetType();

  // Mismatching type
  if (eThis != eOther) return false;

  // Comparators for custom types should be called directly instead of using CVariant methods
  D_ASSERT(eThis >= VAL_INVALID && eThis < VAL_LAST);

  // Variant comparison methods per type
  CVariantCompareFunc aMethods[VAL_LAST] = {
    &CompareInvalid,
    &CompareBool,
    &CompareFloat,
    &CompareInt,
    &CompareString,
    &CompareObject,
    &ComparePtr,
    &CompareVec2,
    &CompareVec3,
    &CompareMat2,
    &CompareMat3,
    &CompareArray,
    &CompareBoolArray,
    &CompareByteArray,
    &CompareIntArray,
    &CompareFloatArray,
    &CompareStrArray,
    &CompareVec2Array,
    &CompareVec3Array,
  };

  // Compare values of the same type
  return aMethods[eThis](*this, valOther);
};

// Comparison
bool CVariant::operator==(const CVariant &valOther) const {
  // Prioritize number types
  const EType eThis = GetNumberType();
  const EType eOther = valOther.GetNumberType();

  if (eThis != VAL_INVALID && eOther != VAL_INVALID) {
    // Prioritize real numbers
    if (eThis == VAL_FLOAT || eOther == VAL_FLOAT) {
      return GetNumber<f64>(*this) == GetNumber<f64>(valOther);
    }

    // Integer types
    return GetNumber<s64>(*this) == GetNumber<s64>(valOther);
  }

  // Compare values of the same main type
  return Compare(valOther);
};

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

}; // namespace dreamy
