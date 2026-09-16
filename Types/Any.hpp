/* Copyright (c) 2022-2026 Dreamy Cecil
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DREAMYUTILITIES_INCL_ANY_H
#define _DREAMYUTILITIES_INCL_ANY_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

#include "Exception.hpp"

#include <algorithm>
#include <typeinfo>

NAMESPACE_DREAMY_OPEN

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
  template<typename Type>
  class CHolder : public CPlaceholder {
    public:
      Type m_value;

    public:
      // Default constructor
      CHolder(const Type &valSet) : m_value(valSet)
      {
      };

      // Get value type
      virtual const std::type_info &GetType() const {
        return typeid(Type);
      };

      // Clone the value
      virtual CPlaceholder *Clone() const {
        return new CHolder(m_value);
      };
  };

public:
  CPlaceholder *m_content; // Currently held value

public:
  // Default constructor
  inline CAny() : m_content(nullptr)
  {
  };

  // Constructor from a value of any type
  template<typename Type>
  inline CAny(const Type &valSet) : m_content(new CHolder<Type>(valSet))
  {
  };

  // Copy constructor
  inline CAny(const CAny &other) : m_content(!other.IsEmpty() ? other.m_content->Clone() : nullptr)
  {
  };

  // Destructor
  ~CAny() {
    delete m_content;
  };

public:
  // Swap values
  inline CAny &Swap(CAny &anyOther) {
    std::swap(m_content, anyOther.m_content);
    return *this;
  };

  // Assign a value of a new type
  template<typename Type>
  inline CAny &operator=(const Type &anyOther) {
    CAny(anyOther).Swap(*this);
    return *this;
  };

  // Assign a value
  inline CAny &operator=(const CAny &anyOther) {
    CAny(anyOther).Swap(*this);
    return *this;
  };

  // Check if value is empty
  inline bool IsEmpty() const {
    return m_content == nullptr;
  };

  // Get value type
  inline const std::type_info &GetType() const {
    return !IsEmpty() ? m_content->GetType() : typeid(void);
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
template<typename Type>
Type *AnyCast(CAny *pValue) {
  return (pValue != nullptr && pValue->GetType() == typeid(Type)) ?
          &static_cast<CAny::CHolder<Type> *>(pValue->m_content)->m_value : nullptr;
};

// Cast any value into a constant pointer to a typed value
template<typename Type>
const Type *AnyCast(const CAny *pValue) {
  return AnyCast<Type>(const_cast<CAny *>(pValue));
};

// Cast any value into a reference to a typed value
template<typename Type>
Type &AnyCast(CAny &value) {
  Type *pResult = AnyCast<Type>(&value);
  if (!pResult) throw CBadAnyCastException();
  return *pResult;
};

// Cast any value into a constant reference to a typed value
template<typename Type>
const Type &AnyCast(const CAny &value) {
  const Type *pResult = AnyCast<Type>(&value);
  if (!pResult) throw CBadAnyCastException();
  return *pResult;
};

NAMESPACE_DREAMY_CLOSE

#endif // (Dreamy Utilities Include Guard)
