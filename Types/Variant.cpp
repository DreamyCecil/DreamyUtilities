//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#include "Variant.hpp"

NAMESPACE_DREAMY_OPEN

// Print variant value ('null' is used in place of undefined values in JSON)
void CVariant::Print(CStringStream &strm, const ValPrintOpts &opts, const c8 *strUndefined) const {
  switch (GetType()) {
    case VAL_BOOL: {
      strm << (ToBool() ? "true" : "false");
    } break;

    case VAL_FLOAT: {
      strm << ToFloat();
    } break;

    case VAL_INT: {
      strm << ToInt();
    } break;

    case VAL_STRING: {
      if (opts.IsInline() && opts.IsWithoutQuotes()) {
        strm << ToString();
      } else {
        strm << '"' << ToString() << '"';
      }
    } break;

    case VAL_VEC2: {
      const vec2d &v = ToVec2();

      if (opts.IsInline() && opts.IsCompact()) {
        strm << '[' << v[0] << ',' << v[1] << ']';
      } else {
        strm << '[' << v[0] << ", " << v[1] << ']';
      }
    } break;

    case VAL_VEC3: {
      const vec3d &v = ToVec3();

      if (opts.IsInline() && opts.IsCompact()) {
        strm << '[' << v[0] << ',' << v[1] << ',' << v[2] << ']';
      } else {
        strm << '[' << v[0] << ", " << v[1] << ", " << v[2] << ']';
      }
    } break;

    case VAL_MAT2: {
      const mat2d &m = ToMat2();
      const vec2d &v0 = m[0];
      const vec2d &v1 = m[1];

      if (opts.IsInline() && opts.IsCompact()) {
        strm << "[[" << v0[0] << ',' << v0[1] << "],["
                     << v1[0] << ',' << v1[1] << "]]";
      } else {
        strm << "[ [" << v0[0] << ", " << v0[1] << "], ["
                      << v1[0] << ", " << v1[1] << "] ]";
      }
    } break;

    case VAL_MAT3: {
      const mat3d &m = ToMat3();
      const vec3d &v0 = m[0];
      const vec3d &v1 = m[1];
      const vec3d &v2 = m[2];

      if (opts.IsInline() && opts.IsCompact()) {
        strm << "[[" << v0[0] << ',' << v0[1] << ',' << v0[2] << "],["
                     << v1[0] << ',' << v1[1] << ',' << v1[2] << "],["
                     << v2[0] << ',' << v2[1] << ',' << v2[2] << "]]";
      } else {
        strm << "[ [" << v0[0] << ", " << v0[1] << ", " << v0[2] << "], ["
                      << v1[0] << ", " << v1[1] << ", " << v1[2] << "], ["
                      << v2[0] << ", " << v2[1] << ", " << v2[2] << "] ]";
      }
    } break;

    case VAL_OBJ: {
      const CValObject &oObject = ToObject();

      // No entries
      if (oObject.size() == 0) {
        strm << "{}";
        break;
      }

      // Open the object
      strm << '{';

      // Character to print after individual elements (newline for the formatted type)
      c8 strNext[2] = "\n";

      // If inline
      if (opts.IsInline()) {
        // Compact mode (nothing after elements)
        strNext[0] = opts.IsCompact() ? '\0' : ' ';
      }

      strm << strNext;

      // Print entries in order
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
          strm << '"' << it->first.GetString() << '"' << (opts.IsCompact() ? ":" : " : ");

        } else {
          // Indent values in the formatted printout
          optsNext.AddIndentation(1);
          strm << optsNext.GetIndentation() << '"' << it->first.GetString() << "\" : ";
        }

        // "key" : <value>, "key" : <value>...
        it->second.Print(strm, optsNext, strUndefined);
      }

      strm << strNext;

      // Original indentation before closing the object
      if (opts.IsFormatted()) strm << opts.GetIndentation();

      // Close the object
      strm << '}';
    } break;

    case VAL_ARR: {
      const CValArray &aArray = ToArray();

      // No entries
      if (aArray.size() == 0) {
        strm << "[]";
        break;
      }

      // Open the array
      strm << '[';

      // Character to print after individual elements (newline for the formatted type)
      c8 strNext[2] = "\n";

      // If inline
      if (opts.IsInline()) {
        // Compact mode (nothing after elements)
        strNext[0] = opts.IsCompact() ? '\0' : ' ';
      }

      strm << strNext;

      // Print entries in order
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
          optsNext.AddIndentation(1);
          strm << optsNext.GetIndentation();
        }

        // <value>, <value>...
        it->Print(strm, optsNext, strUndefined);
      }

      strm << strNext;

      // Original indentation before closing the array
      if (opts.IsFormatted()) strm << opts.GetIndentation();

      // Close the array
      strm << ']';
    } break;

    case VAL_ARR_BOOL: {
      // Add array prefix and convert to array of any type
      strm << "ab";
      CValArray aPrint;
      ToAnyArray(aPrint, ToBoolArray());
      CVariant(aPrint).Print(strm, opts, strUndefined);
    } break;

    case VAL_ARR_BYTE: {
      // Add array prefix and convert to array of any type
      strm << "ac";
      CValArray aPrint;
      ToAnyArray(aPrint, ToByteArray());
      CVariant(aPrint).Print(strm, opts, strUndefined);
    } break;

    case VAL_ARR_INT: {
      // Add array prefix and convert to array of any type
      strm << "ai";
      CValArray aPrint;
      ToAnyArray(aPrint, ToIntArray());
      CVariant(aPrint).Print(strm, opts, strUndefined);
    } break;

    case VAL_ARR_FLOAT: {
      // Add array prefix and convert to array of any type
      strm << "af";
      CValArray aPrint;
      ToAnyArray(aPrint, ToFloatArray());
      CVariant(aPrint).Print(strm, opts, strUndefined);
    } break;

    case VAL_ARR_STR: {
      // Add array prefix and convert to array of any type
      strm << "as";
      CValArray aPrint;
      ToAnyArray(aPrint, ToStrArray());
      CVariant(aPrint).Print(strm, opts, strUndefined);
    } break;

    case VAL_ARR_VEC2: {
      // Add array prefix and convert to array of any type
      strm << "av";
      CValArray aPrint;
      ToAnyArray(aPrint, ToVec2Array());
      CVariant(aPrint).Print(strm, opts, strUndefined);
    } break;

    case VAL_ARR_VEC3: {
      // Add array prefix and convert to array of any type
      strm << "av";
      CValArray aPrint;
      ToAnyArray(aPrint, ToVec3Array());
      CVariant(aPrint).Print(strm, opts, strUndefined);
    } break;

    case VAL_INVALID:
    default: {
      strm << strUndefined;
    } break;
  }
};

// Compare main types directly
bool CVariant::Compare(const CVariant &valOther) const {
  const EType eThis = GetType();
  const EType eOther = valOther.GetType();

  // Mismatching type
  if (eThis != eOther) return false;

  // Compare values of the same type
  switch (eThis) {
    case VAL_INVALID:   return true;
    case VAL_BOOL:      return ToBool()       == valOther.ToBool();
    case VAL_FLOAT:     return ToFloat()      == valOther.ToFloat();
    case VAL_INT:       return ToInt()        == valOther.ToInt();
    case VAL_STRING:    return ToString()     == valOther.ToString();
    case VAL_VEC2:      return ToVec2()       == valOther.ToVec2();
    case VAL_VEC3:      return ToVec3()       == valOther.ToVec3();
    case VAL_MAT2:      return ToMat2()       == valOther.ToMat2();
    case VAL_MAT3:      return ToMat3()       == valOther.ToMat3();
    case VAL_OBJ:       return ToObject()     == valOther.ToObject();
    case VAL_ARR:       return ToArray()      == valOther.ToArray();
    case VAL_ARR_BOOL:  return ToBoolArray()  == valOther.ToBoolArray();
    case VAL_ARR_BYTE:  return ToByteArray()  == valOther.ToByteArray();
    case VAL_ARR_INT:   return ToIntArray()   == valOther.ToIntArray();
    case VAL_ARR_FLOAT: return ToFloatArray() == valOther.ToFloatArray();
    case VAL_ARR_STR:   return ToStrArray()   == valOther.ToStrArray();
    case VAL_ARR_VEC2:  return ToVec2Array()  == valOther.ToVec2Array();
    case VAL_ARR_VEC3:  return ToVec3Array()  == valOther.ToVec3Array();
    default: break;
  }

  // Invalid comparison
  return false;
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

NAMESPACE_DREAMY_CLOSE
