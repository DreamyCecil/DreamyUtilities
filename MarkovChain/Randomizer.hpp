//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_MARKOV_RANDOMIZER_H
#define _DREAMYUTILITIES_INCL_MARKOV_RANDOMIZER_H

#include "../Base/Base.hpp"

#include "../MarkovChain/MarkovBase.hpp"

// Random algorithms
#if defined(_DREAMY_BOOST_RANDOM_DEVICE)
  #include <boost/random/uniform_int.hpp>
  #include <boost/random/uniform_real.hpp>
  #include <boost/random/mersenne_twister.hpp>
  #include <boost/nondet_random.hpp>

#elif _DREAMY_CPP11
  #include <random>
#endif

namespace dreamy {

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

#elif _DREAMY_CPP11
  #if !_DREAMY_UNIX
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
#endif

};

#endif // (Dreamy Utilities Include Guard)
