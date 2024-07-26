//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_VARIANT_H
#define _DREAMYUTILITIES_INCL_VARIANT_H

#include "../DreamyUtilitiesBase.hpp"

#include "Any.hpp"
#include "Arrays.hpp"
#include "HashedString.hpp"

// Extra types
#include "../IO/StringStream.hpp"
#include "../Math/ScalarMatrices.hpp"
#include "../Math/ScalarVectors.hpp"

// Value containers
#include <vector>
#include "UnorderedMap.hpp"

namespace dreamy {

class CVariant; // Pre-define variant

typedef std::vector<CVariant>                          CValArray;  // Array of values
typedef dreamy::unordered_map<CHashedString, CVariant> CValObject; // Variable map
typedef std::pair<CHashedString, CVariant>             CValPair;   // Key-value pair

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
  void FuncName(const CVariant &val, CStringStream &strm, const ValPrintOpts &opts, const c8 *strUndefined)

// Printing method function type
typedef VARIANT_DECLARE_PRINT((*CVariantPrintFunc));

// Declare type-specific printing methods
VARIANT_DECLARE_PRINT(PrintInvalid);

VARIANT_DECLARE_PRINT(PrintBool);
VARIANT_DECLARE_PRINT(PrintFloat);
VARIANT_DECLARE_PRINT(PrintInt);
VARIANT_DECLARE_PRINT(PrintString);

VARIANT_DECLARE_PRINT(PrintObject);
VARIANT_DECLARE_PRINT(PrintPtr);

VARIANT_DECLARE_PRINT(PrintVec2);
VARIANT_DECLARE_PRINT(PrintVec3);
VARIANT_DECLARE_PRINT(PrintMat2);
VARIANT_DECLARE_PRINT(PrintMat3);

VARIANT_DECLARE_PRINT(PrintArray);
VARIANT_DECLARE_PRINT(PrintBoolArray);
VARIANT_DECLARE_PRINT(PrintByteArray);
VARIANT_DECLARE_PRINT(PrintIntArray);
VARIANT_DECLARE_PRINT(PrintFloatArray);
VARIANT_DECLARE_PRINT(PrintStrArray);
VARIANT_DECLARE_PRINT(PrintVec2Array);
VARIANT_DECLARE_PRINT(PrintVec3Array);

// Define methods for a full type
#define VARIANT_TYPE_METHODS(ArgumentType, ValueType, TypeIndex, FuncIdentifier) \
  /* Type constructor */ \
  CVariant(ArgumentType valSet) { From##FuncIdentifier(valSet); } \
  /* Type assignment (method instead of 'operator=' to avoid confusion between the class and its types) */ \
  inline void From##FuncIdentifier(ArgumentType valSet) { _type = (EType)TypeIndex; _val = valSet; _print = &Print##FuncIdentifier; } \
  /* Type casting */ \
  inline       ValueType &To##FuncIdentifier(void)       { return AnyCast<ValueType>(_val); } \
  inline const ValueType &To##FuncIdentifier(void) const { return AnyCast<ValueType>(_val); }

// Define methods for a pointer type
#define VARIANT_PTR_METHODS(ValueType, TypeIndex, FuncIdentifier) \
  /* Type constructor */ \
  CVariant(ValueType valSet) { From##FuncIdentifier(valSet); } \
  /* Type assignment (method instead of 'operator=' to avoid confusion between the class and its types) */ \
  inline void From##FuncIdentifier(ValueType valSet) { _type = (EType)TypeIndex; _val = valSet; _print = &Print##FuncIdentifier; } \
  /* Type casting */ \
  inline ValueType To##FuncIdentifier(void) const { return AnyCast<ValueType>(_val); }

// Define a global method for converting arrays of some type into an array of variants
#define VARIANT_CONVERT_ARRAY_METHOD(ArrayType, TypeName) \
  inline void ToAnyArray(CValArray &aToArray, const ArrayType &aFromArray) { \
    /* Allocate enough space */ \
    s32 iElements = (s32)aFromArray.size(); \
    aToArray.resize(iElements); \
    /* Copy elements one by one */ \
    while (--iElements >= 0) aToArray[iElements].From##TypeName(aFromArray[iElements]); \
  }

// Class that houses a value of any type
class CAbstractValue {

public:
  // Main value types
  enum EType {
    // No specific value
    VAL_INVALID,

    // Atomic types
    VAL_BOOL,   // First bit of an 8-bit integer
    VAL_FLOAT,  // 64-bit float
    VAL_INT,    // 64-bit signed integer
    VAL_STRING, // ASCII string

    // Special types
    VAL_OBJ, // Object of string-variant pairs
    VAL_PTR, // Pointer to another variant

    // Math types
    VAL_VEC2,
    VAL_VEC3,
    VAL_MAT2,
    VAL_MAT3,

    // Array types
    VAL_ARR,       // Variants
    VAL_ARR_BOOL,  // Space-efficient bits
    VAL_ARR_BYTE,  // Bytes
    VAL_ARR_INT,   // Integers
    VAL_ARR_FLOAT, // Real numbers
    VAL_ARR_STR,   // Strings
    VAL_ARR_VEC2,  // 2D vectors
    VAL_ARR_VEC3,  // 3D vectors

    // Last type for offsetting
    VAL_LAST,
  };

protected:
  EType _type; // Value type index
  CAny _val; // Actual value
  CVariantPrintFunc _print; // Printing method associated with the type

public:
  // Default constructor
  CAbstractValue() : _type(VAL_INVALID), _val(s64(0)), _print(&PrintInvalid)
  {
  };

  // Copy constructor
  CAbstractValue(const CAbstractValue &valOther) :
    _type(valOther._type), _val(valOther._val), _print(valOther._print)
  {
  };

  // Get value type
  inline EType GetType(void) const {
    return _type;
  };

  // Get actual value
  inline const CAny &GetValue(void) const {
    return _val;
  };

  // Custom assignment
  inline void Set(s32 iType, const CAny &valSet, CVariantPrintFunc pPrintSet) {
    _type = (EType)iType;
    _val = valSet;
    _print = pPrintSet;
  };

  // Assignment
  CAbstractValue &operator=(const CAbstractValue &valOther) {
    if (&valOther == this) return *this;

    _type = valOther._type;
    _val = valOther._val;
    _print = valOther._print;
    return *this;
  };
};

// Class that houses a value of any valid type
class CVariant : public CAbstractValue {

public:
  // Default constructor
  CVariant() : CAbstractValue()
  {
  };

  // Type-specific methods
  VARIANT_TYPE_METHODS(f64,  f64,  VAL_FLOAT, Float);
  VARIANT_TYPE_METHODS(bool, bool, VAL_BOOL,  Bool);
  VARIANT_TYPE_METHODS(s64,  s64,  VAL_INT,   Int);

  // Different integer types
  CVariant(u8  i) { FromInt(i); };
  CVariant(s8  i) { FromInt(i); };
  CVariant(u16 i) { FromInt(i); };
  CVariant(s16 i) { FromInt(i); };
  CVariant(u32 i) { FromInt(i); };
  CVariant(s32 i) { FromInt(i); };
  CVariant(u64 i) { FromInt(i); };
  CVariant(unsigned long i) { FromInt(i); }; // Unusual case

  // Strings
  VARIANT_TYPE_METHODS(const CString &, CString, VAL_STRING, String);
  CVariant(const c8 *str) { FromString(str); };

  VARIANT_TYPE_METHODS(const CValObject &, CValObject, VAL_OBJ, Object);
  VARIANT_PTR_METHODS(CVariant *, VAL_PTR, Ptr);

  VARIANT_TYPE_METHODS(const vec2d &, vec2d, VAL_VEC2, Vec2);
  VARIANT_TYPE_METHODS(const vec3d &, vec3d, VAL_VEC3, Vec3);
  VARIANT_TYPE_METHODS(const mat2d &, mat2d, VAL_MAT2, Mat2);
  VARIANT_TYPE_METHODS(const mat3d &, mat3d, VAL_MAT3, Mat3);

  VARIANT_TYPE_METHODS(const CValArray   &, CValArray,   VAL_ARR,       Array);
  VARIANT_TYPE_METHODS(const Bits_t      &, Bits_t,      VAL_ARR_BOOL,  BoolArray);
  VARIANT_TYPE_METHODS(const Bytes_t     &, Bytes_t,     VAL_ARR_BYTE,  ByteArray);
  VARIANT_TYPE_METHODS(const Ints_t      &, Ints_t,      VAL_ARR_INT,   IntArray);
  VARIANT_TYPE_METHODS(const Numbers_t   &, Numbers_t,   VAL_ARR_FLOAT, FloatArray);
  VARIANT_TYPE_METHODS(const Strings_t   &, Strings_t,   VAL_ARR_STR,   StrArray);
  VARIANT_TYPE_METHODS(const Vec2Array_t &, Vec2Array_t, VAL_ARR_VEC2,  Vec2Array);
  VARIANT_TYPE_METHODS(const Vec3Array_t &, Vec3Array_t, VAL_ARR_VEC3,  Vec3Array);

  // Check for a distinctive number type (float, integer or invalid)
  inline EType GetNumberType(void) const {
    const EType eType = GetType();

    if (eType > VAL_INVALID && eType < VAL_STRING) {
      // Real number or integer
      return (eType == VAL_FLOAT) ? VAL_FLOAT : VAL_INT;
    }

    // Not a number
    return VAL_INVALID;
  };

  // Print variant value
  inline void Print(CStringStream &strm, const ValPrintOpts &opts, const c8 *strUndefined = "undefined") const {
    _print(*this, strm, opts, strUndefined);
  };

  // Compare vanilla types directly
  bool Compare(const CVariant &valOther) const;

  // Comparison
  bool operator==(const CVariant &valOther) const;

  // Difference comparison
  inline bool operator!=(const CVariant &valOther) const {
    return !operator==(valOther);
  };
};

// ToAnyArray() methods for converting typed arrays into variant arrays
VARIANT_CONVERT_ARRAY_METHOD(Bits_t,      Bool);
VARIANT_CONVERT_ARRAY_METHOD(Bytes_t,     Int);
VARIANT_CONVERT_ARRAY_METHOD(Ints_t,      Int);
VARIANT_CONVERT_ARRAY_METHOD(Numbers_t,   Float);
VARIANT_CONVERT_ARRAY_METHOD(Strings_t,   String);
VARIANT_CONVERT_ARRAY_METHOD(Vec2Array_t, Vec2);
VARIANT_CONVERT_ARRAY_METHOD(Vec3Array_t, Vec3);

// Retrieve value from a variant of a specific number type
template<typename Type> inline
Type GetNumber(const CVariant &val)
{
  switch (val.GetType()) {
    case CVariant::VAL_BOOL: return static_cast<Type>(val.ToBool());
    case CVariant::VAL_FLOAT: return static_cast<Type>(val.ToFloat());
    case CVariant::VAL_INT: return static_cast<Type>(val.ToInt());
    default: return static_cast<Type>(val.ToInt()); // Throw CBadAnyCastException
  }
};

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

};

#endif // (Dreamy Utilities Include Guard)
