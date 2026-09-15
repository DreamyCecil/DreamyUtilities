//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#include "Variant.hpp"

NAMESPACE_DREAMY_OPEN

inline void PrintFloat(CStringStream &strm, const f64 f) {
  if (dreamy::math::IsNaN(f)) {
    strm << "NaN";
    return;
  }

  s8 iInf = dreamy::math::InfinitySign(f);

  if (iInf == +1) {
    strm << "Infinity";
  } else if (iInf == -1) {
    strm << "-Infinity";
  } else {
    strm << f;
  }
};

inline void PrintCommaSeparator(CStringStream &strm, const ValPrintOpts &opts) {
  if (opts.IsInline() && opts.IsCompact()) {
    strm << ',';
  } else {
    strm << ", ";
  }
};

inline void PrintVec2(CStringStream &strm, const ValPrintOpts &opts, const vec2d &v) {
  strm << '[';
  PrintFloat(strm, v[0]);
  PrintCommaSeparator(strm, opts);
  PrintFloat(strm, v[1]);
  strm << ']';
};

inline void PrintVec3(CStringStream &strm, const ValPrintOpts &opts, const vec3d &v) {
  strm << '[';
  PrintFloat(strm, v[0]);
  PrintCommaSeparator(strm, opts);
  PrintFloat(strm, v[1]);
  PrintCommaSeparator(strm, opts);
  PrintFloat(strm, v[2]);
  strm << ']';
};

// Print variant value into a string stream
void CVariant::Print(CStringStream &strm, const ValPrintOpts &opts) const {
  switch (GetType()) {
    case VAL_BOOL: {
      strm << (AsBool() ? "true" : "false");
    } break;

    case VAL_FLOAT: {
      PrintFloat(strm, AsFloat());
    } break;

    case VAL_INT: {
      strm << AsInt();
    } break;

    case VAL_STRING: {
      strm << AsString();
    } break;

    case VAL_VEC2: {
      PrintVec2(strm, opts, AsVec2());
    } break;

    case VAL_VEC3: {
      PrintVec3(strm, opts, AsVec3());
    } break;

    case VAL_MAT2: {
      const mat2d &m = AsMat2();

      if (opts.IsInline() && opts.IsCompact()) {
        strm << '[';
      } else {
        strm << "[ ";
      }

      PrintVec2(strm, opts, m[0]);
      PrintCommaSeparator(strm, opts);
      PrintVec2(strm, opts, m[1]);

      if (opts.IsInline() && opts.IsCompact()) {
        strm << ']';
      } else {
        strm << " ]";
      }
    } break;

    case VAL_MAT3: {
      const mat3d &m = AsMat3();

      if (opts.IsInline() && opts.IsCompact()) {
        strm << '[';
      } else {
        strm << "[ ";
      }

      PrintVec3(strm, opts, m[0]);
      PrintCommaSeparator(strm, opts);
      PrintVec3(strm, opts, m[1]);
      PrintCommaSeparator(strm, opts);
      PrintVec3(strm, opts, m[2]);

      if (opts.IsInline() && opts.IsCompact()) {
        strm << ']';
      } else {
        strm << " ]";
      }
    } break;

    case VAL_DICT: {
      const CDictionary &dict = AsDictionary();

      // No entries
      if (dict.size() == 0) {
        strm << "{}";
        break;
      }

      // Open the dictionary
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
      CDictionary::const_iterator it;

      for (it = dict.begin(); it != dict.end(); ++it) {
        // Separate values
        if (it != dict.begin()) {
          strm << ',';
          strm << strNext;
        }

        ValPrintOpts optsNext(opts);

        const CVariant &valKey = it->first;
        bool bStringKey = (valKey.GetType() == VAL_STRING);

        // Print key name
        if (opts.IsInline()) {
          if (bStringKey) strm << '"';
          valKey.Print(strm);
          if (bStringKey) strm << '"';

          if (opts.IsCompact()) {
            strm << ':';
          } else {
            strm << " : ";
          }

        } else {
          // Indent values in the formatted printout
          optsNext.AddIndentation(1);
          strm << optsNext.GetIndentation();

          if (bStringKey) strm << '"';
          valKey.Print(strm);
          if (bStringKey) strm << '"';

          strm << " : ";
        }

        // "key" : <value>, "key" : <value>...
        const CVariant &valValue = it->second;
        bool bStringValue = (valValue.GetType() == VAL_STRING);

        if (bStringValue) strm << '"';
        valValue.Print(strm, optsNext);
        if (bStringValue) strm << '"';
      }

      strm << strNext;

      // Original indentation before closing the dictionary
      if (opts.IsFormatted()) strm << opts.GetIndentation();

      // Close the dictionary
      strm << '}';
    } break;

    case VAL_ARR: {
      const CArray &a = AsArray();

      // No entries
      if (a.size() == 0) {
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
      CArray::const_iterator it;

      for (it = a.begin(); it != a.end(); ++it) {
        // Separate values
        if (it != a.begin()) {
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
        bool bString = (it->GetType() == VAL_STRING);

        if (bString) strm << '"';
        it->Print(strm, optsNext);
        if (bString) strm << '"';
      }

      strm << strNext;

      // Original indentation before closing the array
      if (opts.IsFormatted()) strm << opts.GetIndentation();

      // Close the array
      strm << ']';
    } break;

    case VAL_ARR_BOOL: {
      CArray aPrint;
      ToAnyArray(aPrint, AsBoolArray());
      CVariant(aPrint).Print(strm, opts);
    } break;

    case VAL_ARR_BYTE: {
      CArray aPrint;
      ToAnyArray(aPrint, AsByteArray());
      CVariant(aPrint).Print(strm, opts);
    } break;

    case VAL_ARR_INT: {
      CArray aPrint;
      ToAnyArray(aPrint, AsIntArray());
      CVariant(aPrint).Print(strm, opts);
    } break;

    case VAL_ARR_FLOAT: {
      CArray aPrint;
      ToAnyArray(aPrint, AsFloatArray());
      CVariant(aPrint).Print(strm, opts);
    } break;

    case VAL_ARR_STR: {
      CArray aPrint;
      ToAnyArray(aPrint, AsStrArray());
      CVariant(aPrint).Print(strm, opts);
    } break;

    case VAL_ARR_VEC2: {
      CArray aPrint;
      ToAnyArray(aPrint, AsVec2Array());
      CVariant(aPrint).Print(strm, opts);
    } break;

    case VAL_ARR_VEC3: {
      CArray aPrint;
      ToAnyArray(aPrint, AsVec3Array());
      CVariant(aPrint).Print(strm, opts);
    } break;

    case VAL_INVALID:
    default: {
      strm << "null";
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
    case VAL_BOOL:      return AsBool()       == valOther.AsBool();
    case VAL_FLOAT:     return AsFloat()      == valOther.AsFloat();
    case VAL_INT:       return AsInt()        == valOther.AsInt();
    case VAL_STRING:    return AsString()     == valOther.AsString();
    case VAL_VEC2:      return AsVec2()       == valOther.AsVec2();
    case VAL_VEC3:      return AsVec3()       == valOther.AsVec3();
    case VAL_MAT2:      return AsMat2()       == valOther.AsMat2();
    case VAL_MAT3:      return AsMat3()       == valOther.AsMat3();
    case VAL_DICT:      return AsDictionary() == valOther.AsDictionary();
    case VAL_ARR:       return AsArray()      == valOther.AsArray();
    case VAL_ARR_BOOL:  return AsBoolArray()  == valOther.AsBoolArray();
    case VAL_ARR_BYTE:  return AsByteArray()  == valOther.AsByteArray();
    case VAL_ARR_INT:   return AsIntArray()   == valOther.AsIntArray();
    case VAL_ARR_FLOAT: return AsFloatArray() == valOther.AsFloatArray();
    case VAL_ARR_STR:   return AsStrArray()   == valOther.AsStrArray();
    case VAL_ARR_VEC2:  return AsVec2Array()  == valOther.AsVec2Array();
    case VAL_ARR_VEC3:  return AsVec3Array()  == valOther.AsVec3Array();
    default: return false; // Invalid comparison
  }
};

// Comparison
bool CVariant::operator==(const CVariant &valOther) const {
  // Prioritize number types
  const EType eThis = GetNumberType();
  const EType eOther = valOther.GetNumberType();

  if (eThis != VAL_INVALID && eOther != VAL_INVALID) {
    // Prioritize real numbers
    if (eThis == VAL_FLOAT || eOther == VAL_FLOAT) {
      return ToNumber<f64>(*this) == ToNumber<f64>(valOther);
    }

    // Integer types
    return ToNumber<s64>(*this) == ToNumber<s64>(valOther);
  }

  // Compare values of the same main type
  return Compare(valOther);
};

NAMESPACE_DREAMY_CLOSE
