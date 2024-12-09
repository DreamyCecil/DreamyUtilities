//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_MARKOV_CHAIN_H
#define _DREAMYUTILITIES_INCL_MARKOV_CHAIN_H

#include "../DreamyUtilitiesBase.hpp"

#include "MarkovBase.hpp"
#include "UniqueOrder.hpp"
#include "Randomizer.hpp"
#include "../Math/Random.hpp"

namespace dreamy {

// Markov chain dictionary
template<typename Type> class CMarkovChain {

public:
  typedef Type T; // Template type

  #if _DREAMY_CPP11
    typedef std::unordered_map<Type, u32> CValueCounts; // Occurrences of next values
    typedef std::unordered_map<CUniqueOrder<Type>, CValueCounts> CNextValueCounts; // Occurrences of next values per value order

    typedef std::unordered_map<Type, f32> CValueChances; // Chances of next values
    typedef std::unordered_map<CUniqueOrder<Type>, CValueChances> CNextValueChances; // Chances of next values per value order
  #else
    typedef dreamy::unordered_map<Type, u32> CValueCounts; // Occurrences of next values
    typedef dreamy::unordered_map<CUniqueOrder<Type>, CValueCounts> CNextValueCounts; // Occurrences of next values per value order

    typedef dreamy::unordered_map<Type, f32> CValueChances; // Chances of next values
    typedef dreamy::unordered_map<CUniqueOrder<Type>, CValueChances> CNextValueChances; // Chances of next values per value order
  #endif

public:
  CNextValueCounts mch_counts;
  CNextValueChances mch_chances;

#if defined(_DREAMY_BOOST_RANDOM_DEVICE)

private:
  RandomSeed mch_seed; // Seed sequence for randomization
  boost::mt19937 mch_rng; // Random number generator

public:
  // Default constructor
  CMarkovChain() : mch_seed("/dev/random"), mch_rng(mch_seed.begin(), mch_seed.end())
  {
  };

#elif _DREAMY_CPP11

private:
  #if _DREAMY_UNIX
    std::random_device mch_rd;
  #endif
  std::seed_seq mch_seed; // Seed sequence for randomization
  std::mt19937 mch_rng; // Random number generator

public:
  // Default constructor
  CMarkovChain() :
    #if !_DREAMY_UNIX
      mch_seed(GenerateSeedSeq("/dev/random")),
    #else
      mch_rd("/dev/random"),
      mch_seed({mch_rd(), mch_rd(), mch_rd(), mch_rd(), mch_rd(), mch_rd(), mch_rd(), mch_rd()}),
    #endif
    mch_rng(mch_seed)
  {
  };

#else

public:
  // Default constructor
  CMarkovChain() {};

#endif

// Markov chain training
public:

  // Clear markov chain
  void Clear(void) {
    mch_counts.clear();
    mch_chances.clear();
  };

  // Learn a new value
  void RecordOrder(CUniqueOrderContext<Type> &moc, const Type &valNext) {
    // If this order has a full set of observed data
    if (moc.IsObserved(moc.GetSize() - 1)) {
      // Count it
      CValueCounts &mapCounts = mch_counts[moc.GetOrder()];
      ++mapCounts[valNext];
    }

    // Observe next value
    moc.Observe(valNext);
  };

  // Calculate chances of each value order
  void FinalizeLearning(void) {
    // Go through chances of next values of each value order
    typename CNextValueCounts::const_iterator itOrder;

    for (itOrder = mch_counts.begin(); itOrder != mch_counts.end(); ++itOrder)
    {
      u32 iSum = 0;

      typename CValueCounts::const_iterator itNext;

      // Count sum of all values
      for (itNext = itOrder->second.begin(); itNext != itOrder->second.end(); ++itNext) {
        iSum += itNext->second;
      }

      // Chance for each value to be next
      f32 fNextChance = 0.0f;

      for (itNext = itOrder->second.begin(); itNext != itOrder->second.end(); ++itNext) {
        fNextChance += f32(itNext->second) / f32(iSum);
        mch_chances[itOrder->first][itNext->first] = fNextChance;
      }
    }
  };

// Markov chain generator
public:

  // Get random value order
  CUniqueOrder<Type> GetRandomOrder(void) {
    size_t iObs;

    #if defined(_DREAMY_BOOST_RANDOM_DEVICE)
      // Boost randomization algorithm
      boost::uniform_int<size_t> dist(0, mch_chances.size());
      iObs = dist(mch_rng);

    #elif _DREAMY_CPP11
      // STL randomization algorithm
      std::uniform_int_distribution<size_t> dist(0, mch_chances.size());
      iObs = dist(mch_rng);

    #else
      // Simple randomization
      iObs = rand() % mch_chances.size();
    #endif

    typename CMarkovChain<Type>::CNextValueChances::const_iterator it = mch_chances.begin();
    std::advance(it, iObs);

    return it->first;
  };

  // Get next state by picking a random weighted value
  bool GetNextOrder(CUniqueOrder<Type> &order) {
    const CMarkovChain<Type>::CValueChances &aChances = mch_chances[order];

    // No further observations for this order; causes infinite repetition upon calling GetNextOrder() again
    if (aChances.size() == 0) {
      return false;
    }

    f32 fNextValChance;

    #if defined(_DREAMY_BOOST_RANDOM_DEVICE)
      // Boost randomization algorithm
      boost::uniform_real<f32> distRatio(0.0f, 1.0f);
      fNextValChance = distRatio(mch_rng);

    #elif _DREAMY_CPP11
      // STL randomization algorithm
      std::uniform_real_distribution<f32> distRatio(0.0f, 1.0f);
      fNextValChance = distRatio(mch_rng);

    #else
      // Simple randomization
      fNextValChance = (f32)RandRatio();
    #endif

    typename CMarkovChain<Type>::CValueChances::const_iterator it = aChances.begin();

    while (it != aChances.end() && it->second < fNextValChance) {
      ++it;
    }

    // Replace last value with a new one in the beginning
    order.pop_back();
    order.push_front(it->first);

    return true;
  };
};

}; // namespace dreamy

#endif // (Dreamy Utilities Include Guard)
