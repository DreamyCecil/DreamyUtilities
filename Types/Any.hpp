//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_ANY_H
#define _DREAMYUTILITIES_INCL_ANY_H

#include "../DreamyUtilitiesBase.hpp"

#include "Exception.hpp"

#include <algorithm>
#include <typeinfo>

namespace dreamy {

#define ANY_TEMP template<typename Type>

// Type-safe container for single values of any type
class CAny {

public:
  // Value placeholder
  class CPlaceholder {
    public:
      // Destructor
      virtual ~CPlaceholder()
      {
      };

      // Get value type
      virtual const std::type_info &GetType() const = 0;

      // Clone the value
      virtual CPlaceholder *Clone() const = 0;
  };

  // Value holder of a specific type
  ANY_TEMP class CHolder : public CPlaceholder {
    public:
      Type _value;

    public:
      // Default constructor
      CHolder(const Type &valSet) : _value(valSet)
      {
      };

      // Get value type
      virtual const std::type_info &GetType() const {
        return typeid(Type);
      };

      // Clone the value
      virtual CPlaceholder *Clone() const {
        return new CHolder(_value);
      };
  };

public:
  CPlaceholder *_content; // Currently held value

public:
  // Default constructor
  CAny() : _content(nullptr)
  {
  };

  // Constructor from a value of any type
  ANY_TEMP CAny(const Type &valSet) : _content(new CHolder<Type>(valSet))
  {
  };

  // Copy constructor
  CAny(const CAny &other) : _content(!other.IsEmpty() ? other._content->Clone() : nullptr)
  {
  };

  // Destructor
  ~CAny() {
    delete _content;
  };

public:
  // Swap values
  CAny &Swap(CAny &anyOther) {
    std::swap(_content, anyOther._content);
    return *this;
  };

  // Assign a value of a new type
  ANY_TEMP CAny &operator=(const Type &anyOther) {
    CAny(anyOther).Swap(*this);
    return *this;
  };

  // Assign a value
  CAny &operator=(const CAny &anyOther) {
    CAny(anyOther).Swap(*this);
    return *this;
  };

  // Check if value is empty
  bool IsEmpty() const {
    return _content == nullptr;
  };

  // Get value type
  const std::type_info &GetType() const {
    return !IsEmpty() ? _content->GetType() : typeid(void);
  };
};

// Bad any value cast exception
class CBadAnyCastException : public CException {

public:
  virtual const c8 *What() const DREAMY_NOEXCEPT {
    return "Failed conversion using dreamy::AnyCast";
  };
};

// Cast any value into a pointer to a typed value
ANY_TEMP Type *AnyCast(CAny *pValue) {
  return (pValue != nullptr && pValue->GetType() == typeid(Type)) ?
          &static_cast<CAny::CHolder<Type> *>(pValue->_content)->_value : nullptr;
};

// Cast any value into a constant pointer to a typed value
ANY_TEMP const Type *AnyCast(const CAny *pValue) {
  return AnyCast<Type>(const_cast<CAny *>(pValue));
};

// Cast any value into a reference to a typed value
ANY_TEMP Type &AnyCast(CAny &value) {
  Type *pResult = AnyCast<Type>(&value);
    
  // Couldn't cast the pointer
  if (!pResult) throw CBadAnyCastException();

  return *pResult;
};
  
// Cast any value into a constant reference to a typed value
ANY_TEMP const Type &AnyCast(const CAny &value) {
  const Type *pResult = AnyCast<Type>(&value);

  // Couldn't cast the pointer
  if (!pResult) throw CBadAnyCastException();

  return *pResult;
};

#undef ANY_TEMP

};

#endif // (Dreamy Utilities Include Guard)
