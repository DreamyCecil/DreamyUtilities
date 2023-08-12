//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_VARIANT_H
#define _DREAMYUTILITIES_INCL_VARIANT_H

#include "../Base/Base.hpp"

// Any value type
#include "../Types/Any.hpp"

// Hashed variable type
#include "../Types/HashedString.hpp"

// Extra types
#include "../Types/Arrays.hpp"
#include "../Math/ScalarMatrices.hpp"
#include "../Math/ScalarVectors.hpp"

// Value containers
#include <vector>
#include <../Types/UnorderedMap.hpp>

namespace dreamy {

class CVariant; // Pre-define variant

typedef std::vector<CVariant>                          CValArray;  // Array of values
typedef dreamy::unordered_map<CHashedString, CVariant> CValObject; // Variable map
typedef std::pair<CHashedString, CVariant>             CValPair;   // Key-value pair

// Define methods for a type
#define VARIANT_TYPE_METHODS(ArgumentType, ValueType, TypeIndex, FuncIdentifier) \
  /* Type constructor */ \
  /*CVariant(ArgumentType valSet) { From##FuncIdentifier(valSet); }*/ \
  /* Type assignment (method instead of 'operator=' to avoid confusion between the class and its types) */ \
  inline void From##FuncIdentifier(ArgumentType valSet) { _type = (EType)TypeIndex; _val = valSet; } \
  /* Type casting */ \
  inline       ValueType &To##FuncIdentifier(void)      { return AnyCast<ValueType>(_val); } \
  inline const ValueType To##FuncIdentifier(void) const { return AnyCast<ValueType>(_val); }

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
    VAL_INVALID = 0,

    // Scalar types
    VAL_F32,
    VAL_F64, // JSON real numbers
    VAL_BIT, // Bit switch/boolean
    VAL_U8,  // ASCII/UTF-8 character
    VAL_S8,
    VAL_U16, // UTF-16 character
    VAL_S16,
    VAL_U32, // UTF-32 character
    VAL_S32, // Integer
    VAL_U64,
    VAL_S64, // JSON integers

    // Special types
    VAL_STR, // String of characters
    VAL_OBJ, // Object of string-variant pairs
    VAL_PTR, // Pointer to another variant

    // Dimensional types
    VAL_VEC2, // 1x2 array
    VAL_VEC3, // 1x3 array
    VAL_MAT2, // 2x2 array
    VAL_MAT3, // 3x3 array

    // Array types
    VAL_ARR,      // Variants
    VAL_ARR_BIT,  // Space-efficient bits
    VAL_ARR_U8,   // Bytes
    VAL_ARR_S64,  // Integers
    VAL_ARR_F64,  // Real numbers
    VAL_ARR_STR,  // Strings
    VAL_ARR_VEC2, // 2D vectors
    VAL_ARR_VEC3, // 3D vectors

    // Last type for offsetting
    VAL_LAST,
  };

protected:
  EType _type; // Value type index
  CAny _val; // Actual value

public:
  // Default constructor
  CAbstractValue() : _type(VAL_INVALID), _val(s32(0))
  {
  };

  // Copy constructor
  CAbstractValue(const CAbstractValue &valOther) :
    _type(valOther._type), _val(valOther._val)
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
  inline void Set(s32 iType, const CAny &valSet) {
    _type = (EType)iType;
    _val = valSet;
  };

  // Assignment
  CAbstractValue &operator=(const CAbstractValue &valOther) {
    if (&valOther == this) return *this;

    _type = valOther._type;
    _val = valOther._val;
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
  VARIANT_TYPE_METHODS(f32,  f32,  VAL_F32, F32);
  VARIANT_TYPE_METHODS(f64,  f64,  VAL_F64, F64);
  VARIANT_TYPE_METHODS(bool, bool, VAL_BIT, Bit);
  VARIANT_TYPE_METHODS(u8,   u8,   VAL_U8,  U8);
  VARIANT_TYPE_METHODS(s8,   s8,   VAL_S8,  S8);
  VARIANT_TYPE_METHODS(u16,  u16,  VAL_U16, U16);
  VARIANT_TYPE_METHODS(s16,  s16,  VAL_S16, S16);
  VARIANT_TYPE_METHODS(u32,  u32,  VAL_U32, U32);
  VARIANT_TYPE_METHODS(s32,  s32,  VAL_S32, S32);
  VARIANT_TYPE_METHODS(u64,  u64,  VAL_U64, U64);
  VARIANT_TYPE_METHODS(s64,  s64,  VAL_S64, S64);

  VARIANT_TYPE_METHODS(const Str_t      &, Str_t,      VAL_STR, String);
  VARIANT_TYPE_METHODS(const CValObject &, CValObject, VAL_OBJ, Object);
  VARIANT_TYPE_METHODS(      CVariant   *, CVariant *, VAL_PTR, Ptr);

  VARIANT_TYPE_METHODS(const vec2d &, vec2d, VAL_VEC2, Vec2);
  VARIANT_TYPE_METHODS(const vec3d &, vec3d, VAL_VEC3, Vec3);
  VARIANT_TYPE_METHODS(const mat2d &, mat2d, VAL_MAT2, Mat2);
  VARIANT_TYPE_METHODS(const mat3d &, mat3d, VAL_MAT3, Mat3);

  VARIANT_TYPE_METHODS(const CValArray   &, CValArray,   VAL_ARR,      Array);
  VARIANT_TYPE_METHODS(const Bits_t      &, Bits_t,      VAL_ARR_BIT,  BitArray);
  VARIANT_TYPE_METHODS(const Bytes_t     &, Bytes_t,     VAL_ARR_U8,   ByteArray);
  VARIANT_TYPE_METHODS(const Ints_t      &, Ints_t,      VAL_ARR_S64,  IntArray);
  VARIANT_TYPE_METHODS(const Numbers_t   &, Numbers_t,   VAL_ARR_F64,  NumArray);
  VARIANT_TYPE_METHODS(const Strings_t   &, Strings_t,   VAL_ARR_STR,  StrArray);
  VARIANT_TYPE_METHODS(const Vec2Array_t &, Vec2Array_t, VAL_ARR_VEC2, Vec2Array);
  VARIANT_TYPE_METHODS(const Vec3Array_t &, Vec3Array_t, VAL_ARR_VEC3, Vec3Array);

  // Check for a distinctive number type (float, integer or invalid)
  inline EType GetNumberType(void) const {
    const EType eType = GetType();

    if (eType > VAL_INVALID && eType < VAL_STR) {
      // Real number or integer
      return (eType < VAL_BIT) ? VAL_F64 : VAL_S64;
    }

    // Not a number
    return VAL_INVALID;
  };

  // Compare vanilla types directly
  inline bool Compare(const CVariant &valOther) const;

  // Comparison
  inline bool operator==(const CVariant &valOther) const;

  // Difference comparison
  inline bool operator!=(const CVariant &valOther) const {
    return !operator==(valOther);
  };
};

// ToAnyArray() methods for converting typed arrays into variant arrays
VARIANT_CONVERT_ARRAY_METHOD(Bits_t,      Bit);
VARIANT_CONVERT_ARRAY_METHOD(Bytes_t,     U8);
VARIANT_CONVERT_ARRAY_METHOD(Ints_t,      S64);
VARIANT_CONVERT_ARRAY_METHOD(Numbers_t,   F64);
VARIANT_CONVERT_ARRAY_METHOD(Strings_t,   String);
VARIANT_CONVERT_ARRAY_METHOD(Vec2Array_t, Vec2);
VARIANT_CONVERT_ARRAY_METHOD(Vec3Array_t, Vec3);

// Retrieve value from a variant of a specific number type
template<typename Type> inline
Type GetNumber(const CVariant &val)
{
  switch (val.GetType()) {
    case CVariant::VAL_F32: return static_cast<Type>(val.ToF32());
    case CVariant::VAL_F64: return static_cast<Type>(val.ToF64());
    case CVariant::VAL_BIT: return static_cast<Type>(val.ToBit());
    case CVariant::VAL_U8:  return static_cast<Type>(val.ToU8());
    case CVariant::VAL_S8:  return static_cast<Type>(val.ToS8());
    case CVariant::VAL_U16: return static_cast<Type>(val.ToU16());
    case CVariant::VAL_S16: return static_cast<Type>(val.ToS16());
    case CVariant::VAL_U32: return static_cast<Type>(val.ToU32());
    case CVariant::VAL_S32: return static_cast<Type>(val.ToS32());

    case CVariant::VAL_U64:
      // Old MSVC compilers don't support unsigned 64-bit integer casting into double
      #if !_DREAMY_UNIX && !_DREAMY_CPP11
        return static_cast<Type>((s64)val.ToU64());
      #else
        return static_cast<Type>(val.ToU64());
      #endif

    case CVariant::VAL_S64: return static_cast<Type>(val.ToS64());

    // Throw CBadAnyCastException
    default: return static_cast<Type>(val.ToU8());
  }
};

};

// Variant comparison
#include "../Variant/Comparison.hpp"

// Variant printout in JSON format
#include "../Variant/Printing.hpp"

#endif // (Dreamy Utilities Include Guard)
