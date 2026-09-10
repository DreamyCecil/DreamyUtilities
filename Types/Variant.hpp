//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_VARIANT_H
#define _DREAMYUTILITIES_INCL_VARIANT_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

#include "Any.hpp"
#include "Arrays.hpp"
#include "HashedString.hpp"
#include "../Math/Algorithm.hpp"

// Extra types
#include "../IO/StringStream.hpp"
#include "../Math/ScalarMatrices.hpp"
#include "../Math/ScalarVectors.hpp"

// Value containers
#include <vector>
#include "UnorderedMap.hpp"

NAMESPACE_DREAMY_OPEN

class CVariant; // Pre-define variant

typedef std::vector<CVariant>                     CArray; // Array of variants
typedef dreamy::unordered_map<CVariant, CVariant> CDictionary; // Dictionary of key-value pairs
typedef std::pair<CVariant, CVariant>             CPair; // Key-value pair

// Structure that handles options for specific printout of variant types
struct ValPrintOpts {

private:
  // Printout type
  enum EPrintType {
    // Single line (except when they contain newlines, e.g. strings)
    // [arg1 = compact : bool]
    E_INLINE,

    // JSON-like blocks with automatic indentation
    // [arg1 = indentation level : s32]
    // [arg2 = indent with spaces : bool]
    // [arg3 = spaces per tab : s32]
    E_FORMATTED,
  } eType;

  s32 aiArgs[3]; // Printout arguments

  __forceinline ValPrintOpts(const EPrintType eSetType = E_INLINE, s32 iArg1 = 0, s32 iArg2 = 0, s32 iArg3 = 0) : eType(eSetType) {
    aiArgs[0] = iArg1; aiArgs[1] = iArg2; aiArgs[2] = iArg3;
  };

public:
  __forceinline ValPrintOpts(const ValPrintOpts &optsOther) : eType(optsOther.eType) {
    aiArgs[0] = optsOther.aiArgs[0];
    aiArgs[1] = optsOther.aiArgs[1];
    aiArgs[2] = optsOther.aiArgs[2];
  };

  static __forceinline ValPrintOpts Default(void) {
    return ValPrintOpts();
  };

// Inline printing
public:

  static __forceinline ValPrintOpts Inline(bool bCompact) {
    return ValPrintOpts(E_INLINE, bCompact);
  };

  __forceinline bool IsInline(void) const { return eType == E_INLINE; };

  __forceinline bool IsCompact(void) const { D_ASSERT(IsInline()); return !!aiArgs[0]; };

// Formatted printing
public:

  static __forceinline ValPrintOpts Formatted(s32 iIndentationLevel, bool bIndentWithSpaces = false, s32 iSpacesPerTab = 4) {
    return ValPrintOpts(E_FORMATTED, math::Max(iIndentationLevel, 0), bIndentWithSpaces, math::Max(iSpacesPerTab, 1));
  };

  __forceinline bool IsFormatted(void) const { return eType == E_FORMATTED; };

  __forceinline s32 GetIndentLevel(void) const { D_ASSERT(IsFormatted()); return aiArgs[0]; };
  __forceinline bool IsUsingSpaces(void) const { D_ASSERT(IsFormatted()); return !!aiArgs[1]; };
  __forceinline s32 GetSpacesPerTab(void) const { D_ASSERT(IsFormatted()); return aiArgs[2]; };

  inline void AddIndentation(s32 i) {
    D_ASSERT(IsFormatted());
    aiArgs[0] = math::Max(aiArgs[0] + i, 0);
  };

  inline CString GetIndentation(void) const {
    D_ASSERT(IsFormatted());

    // Indent values in the formatted printout with a specific amount of spaces
    if (IsUsingSpaces()) {
      return CString(GetIndentLevel() * GetSpacesPerTab(), ' ');

    // Indent values in the formatted printout with tabs
    } else {
      return CString(GetIndentLevel(), '\t');
    }
  };
};

// Class that houses a value of any valid type
class CVariant {

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

    // Math types
    VAL_VEC2,
    VAL_VEC3,
    VAL_MAT2,
    VAL_MAT3,

    // Container types
    VAL_DICT,      // Dictionary of variant-variant pairs
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

public:
  // Default constructor
  CVariant() : _type(VAL_INVALID), _val(s64(0))
  {
  };

  // Copy constructor
  CVariant(const CVariant &valOther) : _type(valOther._type), _val(valOther._val)
  {
  };

  // Constructors per valid type
  __forceinline CVariant(bool b) { FromBool(b); };
  __forceinline CVariant(f64  f) { FromFloat(f); };
  __forceinline CVariant(u8   i) { FromInt(i); };
  __forceinline CVariant(s8   i) { FromInt(i); };
  __forceinline CVariant(u16  i) { FromInt(i); };
  __forceinline CVariant(s16  i) { FromInt(i); };
  __forceinline CVariant(u32  i) { FromInt(i); };
  __forceinline CVariant(s32  i) { FromInt(i); };
  __forceinline CVariant(u64  i) { FromInt(i); };
  __forceinline CVariant(s64  i) { FromInt(i); };
  __forceinline CVariant(unsigned long i) { FromInt(i); }; // Unusual case
  __forceinline CVariant(const CString &s) { FromString(s); };
  __forceinline CVariant(const c8 *s) { FromString(s); };
  __forceinline CVariant(const vec2d &v) { FromVec2(v); };
  __forceinline CVariant(const vec3d &v) { FromVec3(v); };
  __forceinline CVariant(const mat2d &m) { FromMat2(m); };
  __forceinline CVariant(const mat3d &m) { FromMat3(m); };
  __forceinline CVariant(const CDictionary &d) { FromDictionary(d); };
  __forceinline CVariant(const CArray      &a) { FromArray(a); };
  __forceinline CVariant(const Bits_t      &a) { FromBoolArray(a); };
  __forceinline CVariant(const Bytes_t     &a) { FromByteArray(a); };
  __forceinline CVariant(const Ints_t      &a) { FromIntArray(a); };
  __forceinline CVariant(const Numbers_t   &a) { FromFloatArray(a); };
  __forceinline CVariant(const Strings_t   &a) { FromStrArray(a); };
  __forceinline CVariant(const Vec2Array_t &a) { FromVec2Array(a); };
  __forceinline CVariant(const Vec3Array_t &a) { FromVec3Array(a); };

  // Creation from valid types
  inline void FromBool      (const bool         b) { _type = VAL_BOOL;      _val = b; };
  inline void FromFloat     (const f64          f) { _type = VAL_FLOAT;     _val = f; };
  inline void FromInt       (const s64          i) { _type = VAL_INT;       _val = i; };
  inline void FromString    (const CString     &s) { _type = VAL_STRING;    _val = s; };
  inline void FromVec2      (const vec2d       &v) { _type = VAL_VEC2;      _val = v; };
  inline void FromVec3      (const vec3d       &v) { _type = VAL_VEC3;      _val = v; };
  inline void FromMat2      (const mat2d       &v) { _type = VAL_MAT2;      _val = v; };
  inline void FromMat3      (const mat3d       &v) { _type = VAL_MAT3;      _val = v; };
  inline void FromDictionary(const CDictionary &d) { _type = VAL_DICT;      _val = d; };
  inline void FromArray     (const CArray      &a) { _type = VAL_ARR;       _val = a; };
  inline void FromBoolArray (const Bits_t      &a) { _type = VAL_ARR_BOOL;  _val = a; };
  inline void FromByteArray (const Bytes_t     &a) { _type = VAL_ARR_BYTE;  _val = a; };
  inline void FromIntArray  (const Ints_t      &a) { _type = VAL_ARR_INT;   _val = a; };
  inline void FromFloatArray(const Numbers_t   &a) { _type = VAL_ARR_FLOAT; _val = a; };
  inline void FromStrArray  (const Strings_t   &a) { _type = VAL_ARR_STR;   _val = a; };
  inline void FromVec2Array (const Vec2Array_t &a) { _type = VAL_ARR_VEC2;  _val = a; };
  inline void FromVec3Array (const Vec3Array_t &a) { _type = VAL_ARR_VEC3;  _val = a; };

  // Direct casting to values
  inline bool        &AsBool      (void) { return AnyCast<bool>(_val); };
  inline f64         &AsFloat     (void) { return AnyCast<f64>(_val); };
  inline s64         &AsInt       (void) { return AnyCast<s64>(_val); };
  inline CString     &AsString    (void) { return AnyCast<CString>(_val); };
  inline vec2d       &AsVec2      (void) { return AnyCast<vec2d>(_val); };
  inline vec3d       &AsVec3      (void) { return AnyCast<vec3d>(_val); };
  inline mat2d       &AsMat2      (void) { return AnyCast<mat2d>(_val); };
  inline mat3d       &AsMat3      (void) { return AnyCast<mat3d>(_val); };
  inline CDictionary &AsDictionary(void) { return AnyCast<CDictionary>(_val); };
  inline CArray      &AsArray     (void) { return AnyCast<CArray>(_val); };
  inline Bits_t      &AsBoolArray (void) { return AnyCast<Bits_t>(_val); };
  inline Bytes_t     &AsByteArray (void) { return AnyCast<Bytes_t>(_val); };
  inline Ints_t      &AsIntArray  (void) { return AnyCast<Ints_t>(_val); };
  inline Numbers_t   &AsFloatArray(void) { return AnyCast<Numbers_t>(_val); };
  inline Strings_t   &AsStrArray  (void) { return AnyCast<Strings_t>(_val); };
  inline Vec2Array_t &AsVec2Array (void) { return AnyCast<Vec2Array_t>(_val); };
  inline Vec3Array_t &AsVec3Array (void) { return AnyCast<Vec3Array_t>(_val); };

  // Casting to read-only values
  inline       bool         AsBool      (void) const { return AnyCast<bool>(_val); };
  inline       f64          AsFloat     (void) const { return AnyCast<f64>(_val); };
  inline       s64          AsInt       (void) const { return AnyCast<s64>(_val); };
  inline const CString     &AsString    (void) const { return AnyCast<CString>(_val); };
  inline const vec2d       &AsVec2      (void) const { return AnyCast<vec2d>(_val); };
  inline const vec3d       &AsVec3      (void) const { return AnyCast<vec3d>(_val); };
  inline const mat2d       &AsMat2      (void) const { return AnyCast<mat2d>(_val); };
  inline const mat3d       &AsMat3      (void) const { return AnyCast<mat3d>(_val); };
  inline const CDictionary &AsDictionary(void) const { return AnyCast<CDictionary>(_val); };
  inline const CArray      &AsArray     (void) const { return AnyCast<CArray>(_val); };
  inline const Bits_t      &AsBoolArray (void) const { return AnyCast<Bits_t>(_val); };
  inline const Bytes_t     &AsByteArray (void) const { return AnyCast<Bytes_t>(_val); };
  inline const Ints_t      &AsIntArray  (void) const { return AnyCast<Ints_t>(_val); };
  inline const Numbers_t   &AsFloatArray(void) const { return AnyCast<Numbers_t>(_val); };
  inline const Strings_t   &AsStrArray  (void) const { return AnyCast<Strings_t>(_val); };
  inline const Vec2Array_t &AsVec2Array (void) const { return AnyCast<Vec2Array_t>(_val); };
  inline const Vec3Array_t &AsVec3Array (void) const { return AnyCast<Vec3Array_t>(_val); };

public:
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

  // Print variant value into a string stream
  void Print(CStringStream &strm, const ValPrintOpts &opts = ValPrintOpts::Default()) const;

  // Compare vanilla types directly
  bool Compare(const CVariant &valOther) const;

  // Comparison
  bool operator==(const CVariant &valOther) const;

  // Difference comparison
  inline bool operator!=(const CVariant &valOther) const {
    return !operator==(valOther);
  };

  // Assignment
  CVariant &operator=(const CVariant &valOther) {
    if (&valOther == this) return *this;

    _type = valOther._type;
    _val = valOther._val;
    return *this;
  };
};

// ToAnyArray() methods for converting typed arrays into variant arrays

inline void ToAnyArray(CArray &aToArray, const Bits_t &aFromArray) {
  s32 iElements = (s32)aFromArray.size();
  aToArray.resize(iElements);

  while (--iElements >= 0) {
    aToArray[iElements].FromBool(aFromArray[iElements]);
  }
};

inline void ToAnyArray(CArray &aToArray, const Bytes_t &aFromArray) {
  s32 iElements = (s32)aFromArray.size();
  aToArray.resize(iElements);

  while (--iElements >= 0) {
    aToArray[iElements].FromInt(aFromArray[iElements]);
  }
};

inline void ToAnyArray(CArray &aToArray, const Ints_t &aFromArray) {
  s32 iElements = (s32)aFromArray.size();
  aToArray.resize(iElements);

  while (--iElements >= 0) {
    aToArray[iElements].FromInt(aFromArray[iElements]);
  }
};

inline void ToAnyArray(CArray &aToArray, const Numbers_t &aFromArray) {
  s32 iElements = (s32)aFromArray.size();
  aToArray.resize(iElements);

  while (--iElements >= 0) {
    aToArray[iElements].FromFloat(aFromArray[iElements]);
  }
};

inline void ToAnyArray(CArray &aToArray, const Strings_t &aFromArray) {
  s32 iElements = (s32)aFromArray.size();
  aToArray.resize(iElements);

  while (--iElements >= 0) {
    aToArray[iElements].FromString(aFromArray[iElements]);
  }
};

inline void ToAnyArray(CArray &aToArray, const Vec2Array_t &aFromArray) {
  s32 iElements = (s32)aFromArray.size();
  aToArray.resize(iElements);

  while (--iElements >= 0) {
    aToArray[iElements].FromVec2(aFromArray[iElements]);
  }
};

inline void ToAnyArray(CArray &aToArray, const Vec3Array_t &aFromArray) {
  s32 iElements = (s32)aFromArray.size();
  aToArray.resize(iElements);

  while (--iElements >= 0) {
    aToArray[iElements].FromVec3(aFromArray[iElements]);
  }
};

// Retrieve value from a variant of a specific number type
template<typename Type>
inline Type ToNumber(const CVariant &val) {
  switch (val.GetType()) {
    case CVariant::VAL_BOOL: return static_cast<Type>(val.AsBool());
    case CVariant::VAL_FLOAT: return static_cast<Type>(val.AsFloat());
    case CVariant::VAL_INT: return static_cast<Type>(val.AsInt());
    default: return static_cast<Type>(val.AsInt()); // Throw CBadAnyCastException
  }
};

NAMESPACE_DREAMY_CLOSE

#endif // (Dreamy Utilities Include Guard)
