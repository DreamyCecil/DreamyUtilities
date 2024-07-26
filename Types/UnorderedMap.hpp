//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_UNORDEREDMAP_H
#define _DREAMYUTILITIES_INCL_UNORDEREDMAP_H

#include "../DreamyUtilitiesBase.hpp"

#include <list>
#include <stdexcept>

namespace dreamy {

// Own simple implementation of std::unordered_map for C++98
template<typename Key, typename Type, typename Hash_unused = int, typename KeyEqual_unused = int,
  typename Allocator = std::allocator< std::pair<const Key, Type> > >
class unordered_map : public std::list< std::pair<const Key, Type>, Allocator > {

public:
  // Dreamy Utilities styled typedefs
  typedef Key K;
  typedef Type T;

  // STL styled typedefs
  typedef std::pair<const Key, Type> value_type;
  typedef std::list<value_type> _Myt;
  typedef typename _Myt::iterator iterator;
  typedef typename _Myt::const_iterator const_iterator;
  typedef std::pair<iterator, bool> _Pairib;

public:
  // Beginning of the map
  inline iterator begin(void) {
    return _Myt::begin();
  };

  // Constant beginning of the map
  inline const_iterator begin(void) const {
    return _Myt::begin();
  };

  // End of the map
  inline iterator end(void) {
    return _Myt::end();
  };

  // Constant end of the map
  inline const_iterator end(void) const {
    return _Myt::end();
  };

public:
  // Insert a new pair at the end or find an existing one
  _Pairib insert(const value_type &pair) {
    iterator it = find(pair.first);
    if (it != end()) return std::make_pair(it, false);

    return std::make_pair(_Myt::insert(end(), pair), true);
  };

  // Get iterator to a desired key
  iterator find(const Key &key) {
    for (iterator it = begin(); it != end(); ++it) {
      if (it->first == key) return it;
    }

    return end();
  };

  // Get constant iterator to a desired key
  const_iterator find(const Key &key) const {
    for (const_iterator it = begin(); it != end(); ++it) {
      if (it->first == key) return it;
    }

    return end();
  };

  // Get value under some key
  Type &at(const Key &key) {
    iterator it = find(key);
    if (it != end()) return it->second;

    throw std::range_error("dreamy::unordered_map::at() - key doesn't exist");
  };

  // Get constant value under some key
  const Type &at(const Key &key) const {
    const_iterator it = find(key);
    if (it != end()) return it->second;

    throw std::range_error("dreamy::unordered_map::at() - key doesn't exist");
  };

  // Get value under some key or create a new pair, if there's none
  Type &operator[](const Key &key) {
    iterator it = find(key);
    if (it == end()) it = _Myt::insert(end(), value_type(key, Type()));

    return it->second;
  };

  // Get constant value under some key
  inline const Type &operator[](const Key &key) const {
    return at(key);
  };

  // Count elements under a specific key
  inline size_t count(const Key &key) const {
    size_t ct = 0;

    for (const_iterator it = begin(); it != end(); ++it) {
      if (it->first == key) ++ct;
    }

    return ct;
  };

  // Check if there are any elements under a specific key
  inline bool contains(const Key &key) const {
    return find(key) != end();
  };
};

};

#endif // (Dreamy Utilities Include Guard)
