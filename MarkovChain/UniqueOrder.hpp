//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_MARKOV_UNIQUEORDER_H
#define _DREAMYUTILITIES_INCL_MARKOV_UNIQUEORDER_H

#include "../DreamyUtilitiesBase.hpp"

#include "MarkovBase.hpp"

#include <deque>

namespace dreamy {

// Unique order of values (std::deque abstraction)
template<typename Type> class CUniqueOrder {

public:
  typedef Type T; // Template type

private:
  std::deque<Type> aValues;

public:
  // Default constructor
  __forceinline CUniqueOrder(s32 iSize) : aValues(iSize)
  {
  };

  // Comparison for sorting within std::map
  __forceinline bool operator<(const CUniqueOrder<Type> &vOther) const {
    return aValues < vOther.aValues;
  };

  // Comparison for sorting within std::unordered_map
  __forceinline bool operator==(const CUniqueOrder<Type> &vOther) const {
    return aValues == vOther.aValues;
  };

  // Accessor
  __forceinline Type &operator[](size_t i) {
    return aValues[i];
  };

  // Accessor
  __forceinline Type operator[](size_t i) const {
    return aValues[i];
  };

  // Value count
  __forceinline size_t size(void) const {
    return aValues.size();
  };

  // Remove from the end
  __forceinline void pop_back(void) {
    aValues.pop_back();
  };

  // Insert in the beginning
  __forceinline void push_front(const Type &val) {
    aValues.push_front(val);
  };
};

// Context for the value order
template<typename Type> class CUniqueOrderContext {

public:
  typedef Type T; // Template type

private:
  CUniqueOrder<Type> moc_order;
  std::deque<bool> moc_abObserved;

public:
  // Default constructor
  __forceinline CUniqueOrderContext(u32 iSize) : moc_order(iSize), moc_abObserved(iSize, false)
  {
  };

  // Get unique order
  __forceinline const CUniqueOrder<Type> &GetOrder(void) const {
    return moc_order;
  };

  // Check if observed
  __forceinline bool IsObserved(size_t i) const {
    return moc_abObserved[i];
  };

  // Get context size
  __forceinline size_t GetSize(void) const {
    return moc_order.size();
  };

  // Insert new observed value
  __forceinline void Observe(const Type &val) {
    // Remove last value
    moc_order.pop_back();
    moc_abObserved.pop_back();

    // Observe new value
    moc_order.push_front(val);
    moc_abObserved.push_front(true);
  };
};

};

#if _DREAMY_CPP11
namespace std {

// Unique order hasher
template<typename Type>
struct hash<dreamy::CUniqueOrder<Type>>
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

};
#endif

#endif // (Dreamy Utilities Include Guard)
