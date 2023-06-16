//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_VARIANT_COMPARISON_H
#define _DREAMYUTILITIES_INCL_VARIANT_COMPARISON_H

#include "../Base/Base.hpp"

#include "Variant.hpp"

namespace dreamy
{
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

  VARIANT_COMPARE_METHOD(F32);
  VARIANT_COMPARE_METHOD(F64);
  VARIANT_COMPARE_METHOD(Bit);
  VARIANT_COMPARE_METHOD(U8);
  VARIANT_COMPARE_METHOD(S8);
  VARIANT_COMPARE_METHOD(U16);
  VARIANT_COMPARE_METHOD(S16);
  VARIANT_COMPARE_METHOD(U32);
  VARIANT_COMPARE_METHOD(S32);
  VARIANT_COMPARE_METHOD(U64);
  VARIANT_COMPARE_METHOD(S64);

  VARIANT_COMPARE_METHOD(String);
  VARIANT_COMPARE_METHOD(Object);
  VARIANT_COMPARE_METHOD(Ptr);

  VARIANT_COMPARE_METHOD(Vec2);
  VARIANT_COMPARE_METHOD(Vec3);
  VARIANT_COMPARE_METHOD_CUSTOM(Mat2, CompareMatrices(val1.ToMat2(), val2.ToMat2()));
  VARIANT_COMPARE_METHOD_CUSTOM(Mat3, CompareMatrices(val1.ToMat3(), val2.ToMat3()));

  VARIANT_COMPARE_METHOD(Array);
  VARIANT_COMPARE_METHOD(BitArray);
  VARIANT_COMPARE_METHOD(ByteArray);
  VARIANT_COMPARE_METHOD(IntArray);
  VARIANT_COMPARE_METHOD(NumArray);
  VARIANT_COMPARE_METHOD(StrArray);
  VARIANT_COMPARE_METHOD(Vec2Array);
  VARIANT_COMPARE_METHOD(Vec3Array);

  // Variant comparison methods per type
  typedef std::vector<CVariantCompareFunc> CVariantComparators;

  // Comparator for main types
  static const struct VariantComparator {
    CVariantComparators aMethods;

    VariantComparator(void) {
      aMethods.resize(CVariant::VAL_LAST);
      aMethods[CVariant::VAL_INVALID] = &CompareInvalid;

      aMethods[CVariant::VAL_F32] = &CompareF32;
      aMethods[CVariant::VAL_F64] = &CompareF64;
      aMethods[CVariant::VAL_BIT] = &CompareBit;
      aMethods[CVariant::VAL_U8]  = &CompareU8;
      aMethods[CVariant::VAL_S8]  = &CompareS8;
      aMethods[CVariant::VAL_U16] = &CompareU16;
      aMethods[CVariant::VAL_S16] = &CompareS16;
      aMethods[CVariant::VAL_U32] = &CompareU32;
      aMethods[CVariant::VAL_S32] = &CompareS32;
      aMethods[CVariant::VAL_U64] = &CompareU64;
      aMethods[CVariant::VAL_S64] = &CompareS64;

      aMethods[CVariant::VAL_STR] = &CompareString;
      aMethods[CVariant::VAL_OBJ] = &CompareObject;
      aMethods[CVariant::VAL_PTR] = &ComparePtr;

      aMethods[CVariant::VAL_VEC2] = &CompareVec2;
      aMethods[CVariant::VAL_VEC3] = &CompareVec3;
      aMethods[CVariant::VAL_MAT2] = &CompareMat2;
      aMethods[CVariant::VAL_MAT3] = &CompareMat3;

      aMethods[CVariant::VAL_ARR]      = &CompareArray;
      aMethods[CVariant::VAL_ARR_BIT]  = &CompareBitArray;
      aMethods[CVariant::VAL_ARR_U8]   = &CompareByteArray;
      aMethods[CVariant::VAL_ARR_S64]  = &CompareIntArray;
      aMethods[CVariant::VAL_ARR_F64]  = &CompareNumArray;
      aMethods[CVariant::VAL_ARR_STR]  = &CompareStrArray;
      aMethods[CVariant::VAL_ARR_VEC2] = &CompareVec2Array;
      aMethods[CVariant::VAL_ARR_VEC3] = &CompareVec3Array;
    };
  } _variantComparator;

  // Comparison
  bool CVariant::operator==(const CVariant &valOther) const {
    // Prioritize number types
    const EType eThis = GetNumberType();
    const EType eOther = valOther.GetNumberType();

    if (eThis != VAL_INVALID && eOther != VAL_INVALID) {
      // Prioritize real numbers
      if (eThis == VAL_F64 || eOther == VAL_F64) {
        return GetNumber<f64>(*this) == GetNumber<f64>(valOther);
      }

      // Integer types
      return GetNumber<s64>(*this) == GetNumber<s64>(valOther);
    }

    // Mismatching type
    if (GetType() != valOther.GetType()) return false;

    // Comparators for custom types should be called directly instead of using the '==' operator
    D_ASSERT(GetType() >= VAL_INVALID && GetType() < VAL_LAST);

    // Compare values of the same type
    CVariantCompareFunc pComparator = _variantComparator.aMethods[GetType()];
    return pComparator(*this, valOther);
  };

};

#endif // (Dreamy Utilities Include Guard)
