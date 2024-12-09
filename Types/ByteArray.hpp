//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_BYTEARRAY_H
#define _DREAMYUTILITIES_INCL_BYTEARRAY_H

#include "../DreamyUtilitiesBase.hpp"

namespace dreamy {

// Class that reperesents array of bytes
class CByteArray {

private:
  c8 *_pBuffer;
  size_t _iSize;

public:
  // Default constructor
  CByteArray();

  // Copy constructor
  CByteArray(const CByteArray &baOther);

  // Constructor from a raw array
  CByteArray(const c8 *pData, size_t iSize);

  // Constructor with a specific size
  CByteArray(c8 chByte, size_t iSize);

  // Destructor
  ~CByteArray();

  // Copy data from another byte array
  void Copy(const CByteArray &baOther);

  // Return read-only array of data
  const c8 *ConstData(void) const {
    return _pBuffer;
  };

  // Return array of data
  c8 *Data(void) {
    return _pBuffer;
  };

  // Insert another byte array at a specified position
  inline CByteArray &Insert(size_t iPos, const CByteArray &baOther) {
    // Insert as array of specified size
    return Insert(iPos, baOther.ConstData(), baOther.Size());
  };

  // Insert array of specified size into specified position
  CByteArray &Insert(size_t iPos, const c8 *pData, size_t iSize = NULL_POS);

  // Insert specified amount of specified chars into specified position
  CByteArray &Insert(size_t iPos, size_t iCount, c8 chByte);

  // Remove specified amount of bytes from a certain position
  CByteArray &Remove(size_t iPos, size_t iSize);

  // Change size of allocated memory
  void Resize(size_t iNewSize);

  // Remove specified amound of bytes from the end
  void Chop(size_t iSize);

  // Delete the array buffer
  void Clear(void);

  // Return byte at a position
  c8 At(size_t i) const;

  // Fill array with a specific byte
  CByteArray &Fill(const c8 chByte, size_t iSize = NULL_POS);

  // Return length of the byte array
  inline size_t Size(void) const {
    return _iSize;
  };

  // Check if memory for the array hasn't been allocated
  inline bool IsNull(void) const {
    return (_pBuffer == nullptr);
  };

  // Random access operator
  inline c8 &operator[](size_t i) {
    return Data()[i];
  };

  // Random access operator (read-only)
  inline const c8 &operator[](size_t i) const {
    return ConstData()[i];
  };

  // Duplicate contents of another byte array
  CByteArray &operator=(const CByteArray &baOther);

  // Append content of another byte array
  inline CByteArray &operator+=(const CByteArray &baOther) {
    return Append(baOther);
  };

  // Append zero terminated string
  inline CByteArray &operator+=(const c8 *str) {
    return Append(str);
  };

  // Append specified byte to the end
  inline CByteArray &operator+=(const c8 chByte) {
    return Append(1, chByte);
  };

  // Equality comparison
  bool operator==(const CByteArray &baOther) const;

  // Equality comparison
  bool operator==(const c8 *strOther) const;

  // Inequality comparison
  inline bool operator!=(const CByteArray &baOther) const {
    return !operator==(baOther);
  };

  // Inequality comparison
  inline bool operator!=(const c8 *strOther) const {
    return !operator==(strOther);
  };

  // Swap data with another byte array
  void Swap(CByteArray &baOther);

  // Check if array starts with another byte array
  bool StartsWith(const CByteArray &baOther) const;

  // Check if array starts with a specified byte
  bool StartsWith(const c8 chByte) const;

  // Check if array starts with specified string
  bool StartsWith(const c8 *str) const;

  // Check if array starts with another byte array
  bool EndsWith(const CByteArray &baOther) const;

  // Check if array ends with a specified byte
  bool EndsWith(const c8 chByte) const;

  // Check if array ends with specified string
  bool EndsWith(const c8 *str) const;

  // Return position of first occurence of byte array
  size_t IndexOf(const CByteArray &baOther, size_t iFrom = 0);

  // Return position of the first occurence of a byte
  size_t IndexOf(c8 chByte, size_t iFrom = 0);

  // Return position of first occurence of string
  size_t IndexOf(const c8 *str, size_t iFrom = 0);

  // Return HEX-encoded copy of the array
  CByteArray ToHex(const c8 chDelimiter = '\0') const;

// Method wrappers
public:

  // Insert another byte array at the end
  inline CByteArray &Append(const CByteArray &baOther) {
    return Insert(Size(), baOther);
  };

  // Insert array of specified size at the end
  inline CByteArray &Append(const c8 *pData, size_t iSize = NULL_POS) {
    return Insert(Size(), pData, iSize);
  };

  // Insert specified amount of bytes at the end
  inline CByteArray &Append(size_t iSize, c8 chByte) {
    return Insert(Size(), iSize, chByte);
  };

  // Insert another byte array at the beginning
  inline CByteArray &Prepend(const CByteArray &baOther) {
    return Insert(0, baOther);
  };

  // Insert array of specified size at the beginning
  inline CByteArray &Prepend(const c8 *pData, size_t iSize) {
    return Insert(0, pData, iSize);
  };

  // Insert specified amount of bytes at the beginning
  inline CByteArray &Prepend(size_t iSize, c8 chByte) {
    return Insert(0, iSize, chByte);
  };

  // Return first byte
  inline c8 Front(void) const {
    return ConstData()[0];
  };

  // Return last byte
  inline c8 Back(void) const {
    return ConstData()[Size() - 1];
  };

  // Check if array contains a certain byte
  inline bool Contains(const c8 chByte) {
    return (IndexOf(chByte, 0) != NULL_POS);
  };

  // Check if array contains a sequence from another array
  inline bool Contains(const CByteArray &baOther) {
    return (IndexOf(baOther, 0) != NULL_POS);
  };

  // Check if array contains a string
  inline bool Contains(const c8 *str) {
    return (IndexOf(str, 0) != NULL_POS);
  };
};

}; // namespace dreamy

#endif // (Dreamy Utilities Include Guard)
