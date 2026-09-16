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

#ifndef _DREAMYUTILITIES_INCL_MARKOV_RANDOMIZER_H
#define _DREAMYUTILITIES_INCL_MARKOV_RANDOMIZER_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

#include "MarkovBase.hpp"

// Random algorithms
#if defined(_DREAMY_BOOST_RANDOM_DEVICE)
  #include <boost/random/uniform_int.hpp>
  #include <boost/random/uniform_real.hpp>
  #include <boost/random/mersenne_twister.hpp>
  #include <boost/nondet_random.hpp>

#elif _DREAMY_CPP11
  #include <random>
#endif

NAMESPACE_DREAMY_OPEN

#if defined(_DREAMY_BOOST_RANDOM_DEVICE)
  // Random seed type
  typedef boost::random_device::result_type RandomSeed_t;

  // Random seed sequence
  struct RandomSeed {
    boost::random_device rd; // Boost 1.34.1 only makes it work on Unix!
    RandomSeed_t aSeed[8];

    // Default constructor
    __forceinline RandomSeed(const c8 *strToken) : rd(strToken) {
      // Generate random seed sequence
      for (s32 i = 0; i < 8; ++i) {
        aSeed[i] = rd();
      }
    };

    // Seed sequence beginning
    __forceinline RandomSeed_t *&begin(void) {
      return (RandomSeed_t *&)aSeed;
    };

    // Seed sequence end
    __forceinline RandomSeed_t *end(void) {
      return &aSeed[8];
    };
  };

#elif _DREAMY_CPP11 && !_DREAMY_UNIX
  // Random seed type
  typedef std::random_device::result_type RandomSeed_t;

  // Generate random seed sequence
  inline std::initializer_list<RandomSeed_t> GenerateSeedSeq(const c8 *strSeed) {
    static std::random_device rd(strSeed);

    static RandomSeed_t aResults[8] = {
      rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()
    };

    return std::initializer_list<RandomSeed_t>(&aResults[0], &aResults[8]);
  };
#endif

NAMESPACE_DREAMY_CLOSE

#endif // (Dreamy Utilities Include Guard)
