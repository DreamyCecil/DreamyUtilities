//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_MARKOV_UNIQUEORDER_H
#define _DREAMYUTILITIES_INCL_MARKOV_UNIQUEORDER_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

#include "MarkovBase.hpp"

#include <deque>

NAMESPACE_DREAMY_OPEN

// Unique order of values (std::deque abstraction)
template<typename Type>
class CUniqueOrder {

public:
  typedef Type T; // Template type

private:
  std::deque<Type> m_aValues;

public:
  // Default constructor
  __forceinline CUniqueOrder(s32 iSize) : m_aValues(iSize)
  {
  };

  // Comparison for sorting within std::map
  __forceinline bool operator<(const CUniqueOrder<Type> &vOther) const {
    return m_aValues < vOther.m_aValues;
  };

  // Comparison for sorting within std::unordered_map
  __forceinline bool operator==(const CUniqueOrder<Type> &vOther) const {
    return m_aValues == vOther.m_aValues;
  };

  // Accessor
  __forceinline Type &operator[](size_t i) {
    return m_aValues[i];
  };

  // Accessor
  __forceinline Type operator[](size_t i) const {
    return m_aValues[i];
  };

  // Value count
  __forceinline size_t size(void) const {
    return m_aValues.size();
  };

  // Remove from the end
  __forceinline void pop_back(void) {
    m_aValues.pop_back();
  };

  // Insert in the beginning
  __forceinline void push_front(const Type &val) {
    m_aValues.push_front(val);
  };
};

// Context for the value order
template<typename Type>
class CUniqueOrderContext {

public:
  typedef Type T; // Template type

private:
  CUniqueOrder<Type> m_order;
  std::deque<bool> m_abObserved;

public:
  // Default constructor
  __forceinline CUniqueOrderContext(u32 iSize) : m_order(iSize), m_abObserved(iSize, false)
  {
  };

  // Get unique order
  __forceinline const CUniqueOrder<Type> &GetOrder(void) const {
    return m_order;
  };

  // Check if observed
  __forceinline bool IsObserved(size_t i) const {
    return m_abObserved[i];
  };

  // Get context size
  __forceinline size_t GetSize(void) const {
    return m_order.size();
  };

  // Insert new observed value
  __forceinline void Observe(const Type &val) {
    // Remove last value
    m_order.pop_back();
    m_abObserved.pop_back();

    // Observe new value
    m_order.push_front(val);
    m_abObserved.push_front(true);
  };
};

NAMESPACE_DREAMY_CLOSE

#if _DREAMY_CPP11
namespace std {

// Unique order hasher
template<typename Type>
struct hash<dreamy::CUniqueOrder<Type> >
{
  size_t operator()(const dreamy::CUniqueOrder<Type> &order) const {
    hash<Type> hasher;
    size_t hash = 0;

    const size_t iSize = order.size();

    for (size_t i = 0; i < iSize; ++i) {
      hash = hash * 31 + hasher(order[i]);
    }

    return hash;
  }
};

}; // namespace std
#endif

#endif // (Dreamy Utilities Include Guard)
