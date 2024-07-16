//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_VARIANT_COMPARISON_H
#define _DREAMYUTILITIES_INCL_VARIANT_COMPARISON_H

#include "../Base/Base.hpp"

#include "../Variant/Variant.hpp"

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

};

#endif // (Dreamy Utilities Include Guard)
