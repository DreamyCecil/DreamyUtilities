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

  // Comparison methods per type
  static struct VariantComparator {
    std::vector<CVariantCompareFunc> aMethods;

    // Constructor with main comparators
    VariantComparator(void) {
      aMethods.push_back(&CompareInvalid);

      aMethods.push_back(&CompareF32);
      aMethods.push_back(&CompareF64);
      aMethods.push_back(&CompareBit);
      aMethods.push_back(&CompareU8);
      aMethods.push_back(&CompareS8);
      aMethods.push_back(&CompareU16);
      aMethods.push_back(&CompareS16);
      aMethods.push_back(&CompareU32);
      aMethods.push_back(&CompareS32);
      aMethods.push_back(&CompareU64);
      aMethods.push_back(&CompareS64);

      aMethods.push_back(&CompareString);
      aMethods.push_back(&CompareObject);
      aMethods.push_back(&ComparePtr);

      aMethods.push_back(&CompareVec2);
      aMethods.push_back(&CompareVec3);
      aMethods.push_back(&CompareMat2);
      aMethods.push_back(&CompareMat3);

      aMethods.push_back(&CompareArray);
      aMethods.push_back(&CompareBitArray);
      aMethods.push_back(&CompareByteArray);
      aMethods.push_back(&CompareIntArray);
      aMethods.push_back(&CompareNumArray);
      aMethods.push_back(&CompareStrArray);
      aMethods.push_back(&CompareVec2Array);
      aMethods.push_back(&CompareVec3Array);
    };
  } _variantComparator;

  // Comparison
  bool CVariant::operator==(const CVariant &valOther) const {
    const EType eThisType = GetType();
    const EType eOtherType = valOther.GetType();

    // Scalar types
    if (eThisType & VARIANT_SCALARMASK && eOtherType & VARIANT_SCALARMASK) {
      // Prioritize real numbers
      if (eThisType & VARIANT_NUMBERMASK || eOtherType & VARIANT_NUMBERMASK) {
        return GetNumber<f64>(*this) == GetNumber<f64>(valOther);
      }
      // Integer types
      return GetNumber<s64>(*this) == GetNumber<s64>(valOther);
    }

    // Mismatching type
    if (eThisType != eOtherType) {
      return false;
    }

    // Compare values of the same type
    CVariantCompareFunc pComparator = _variantComparator.aMethods[eThisType];
    return pComparator(*this, valOther);
  };

};

#endif // (Dreamy Utilities Include Guard)
