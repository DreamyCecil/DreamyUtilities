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

#ifndef _DREAMYUTILITIES_INCL_MARKOV_CHAIN_H
#define _DREAMYUTILITIES_INCL_MARKOV_CHAIN_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

#include "MarkovBase.hpp"
#include "UniqueOrder.hpp"
#include "Randomizer.hpp"
#include "../Math/Random.hpp"

NAMESPACE_DREAMY_OPEN

// Markov chain dictionary
template<typename Type>
class CMarkovChain {

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
  CNextValueCounts m_counts;
  CNextValueChances m_chances;

#if defined(_DREAMY_BOOST_RANDOM_DEVICE)

private:
  RandomSeed m_seed; // Seed sequence for randomization
  boost::mt19937 m_rng; // Random number generator

public:
  // Default constructor
  CMarkovChain() : m_seed("/dev/random"), m_rng(m_seed.begin(), m_seed.end())
  {
  };

#elif _DREAMY_CPP11

private:
  #if _DREAMY_UNIX
    std::random_device m_rd;
  #endif
  std::seed_seq m_seed; // Seed sequence for randomization
  std::mt19937 m_rng; // Random number generator

public:
  // Default constructor
  CMarkovChain() :
    #if !_DREAMY_UNIX
      m_seed(GenerateSeedSeq("/dev/random")),
    #else
      m_rd("/dev/random"),
      m_seed({m_rd(), m_rd(), m_rd(), m_rd(), m_rd(), m_rd(), m_rd(), m_rd()}),
    #endif
    m_rng(m_seed)
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
    m_counts.clear();
    m_chances.clear();
  };

  // Learn a new value
  void RecordOrder(CUniqueOrderContext<Type> &moc, const Type &valNext) {
    // If this order has a full set of observed data
    if (moc.IsObserved(moc.GetSize() - 1)) {
      // Count it
      CValueCounts &mapCounts = m_counts[moc.GetOrder()];
      ++mapCounts[valNext];
    }

    // Observe next value
    moc.Observe(valNext);
  };

  // Calculate chances of each value order
  void FinalizeLearning(void) {
    // Go through chances of next values of each value order
    typename CNextValueCounts::const_iterator itOrder;

    for (itOrder = m_counts.begin(); itOrder != m_counts.end(); ++itOrder)
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
        m_chances[itOrder->first][itNext->first] = fNextChance;
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
      boost::uniform_int<size_t> dist(0, m_chances.size());
      iObs = dist(m_rng);

    #elif _DREAMY_CPP11
      // STL randomization algorithm
      std::uniform_int_distribution<size_t> dist(0, m_chances.size());
      iObs = dist(m_rng);

    #else
      // Simple randomization
      iObs = rand() % m_chances.size();
    #endif

    typename CMarkovChain<Type>::CNextValueChances::const_iterator it = m_chances.begin();
    std::advance(it, iObs);

    return it->first;
  };

  // Get next state by picking a random weighted value
  bool GetNextOrder(CUniqueOrder<Type> &order) {
    const CMarkovChain<Type>::CValueChances &aChances = m_chances[order];

    // No further observations for this order; causes infinite repetition upon calling GetNextOrder() again
    if (aChances.size() == 0) {
      return false;
    }

    f32 fNextValChance;

    #if defined(_DREAMY_BOOST_RANDOM_DEVICE)
      // Boost randomization algorithm
      boost::uniform_real<f32> distRatio(0.0f, 1.0f);
      fNextValChance = distRatio(m_rng);

    #elif _DREAMY_CPP11
      // STL randomization algorithm
      std::uniform_real_distribution<f32> distRatio(0.0f, 1.0f);
      fNextValChance = distRatio(m_rng);

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

NAMESPACE_DREAMY_CLOSE

#endif // (Dreamy Utilities Include Guard)
