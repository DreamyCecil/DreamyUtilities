// This file is a part of Dreamy Utilities

#ifndef _DREAMYUTILITIES_INCL_BYTEARRAY_H
#define _DREAMYUTILITIES_INCL_BYTEARRAY_H

#include "../Base/Base.hpp"

namespace dreamy
{
  // Class that reperesents array of bytes
  class CByteArray {
    private:
      c8 *_pBuffer;
      size_t _iSize;

    public:
      // Default constructor
      CByteArray() : _pBuffer(nullptr), _iSize(0)
      {
      };

      // Copy constructor
      CByteArray(const CByteArray &baOther) : _pBuffer(nullptr), _iSize(0)
      {
        Copy(baOther);
      };

      // Constructor from a raw array
      CByteArray(const c8 *pData, size_t iSize) : _pBuffer(nullptr), _iSize(0)
      {
        if (pData == nullptr) return;

        // Calculate length of the array
        if (iSize == NULL_POS) {
          iSize = strlen(pData);
        }

        if (iSize != 0) {
          Resize(iSize);
          memcpy(_pBuffer, pData, iSize);
        }
      };

      // Constructor with a specific size
      CByteArray(c8 chByte, size_t iSize) : _pBuffer(nullptr), _iSize(0)
      {
        if (iSize != 0) {
          Resize(iSize);
          memset(_pBuffer, chByte, iSize);
        }
      };

      // Destructor
      ~CByteArray() {
        Clear();
      };

      // Copy data from another byte array
      void Copy(const CByteArray &baOther) {
        Clear();

        if (!baOther.IsNull() && baOther.Size() != 0) {
          Resize(baOther.Size());
          memcpy(_pBuffer, baOther.ConstData(), baOther.Size());
        }
      };

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
      CByteArray &Insert(size_t iPos, const c8 *pData, size_t iSize = NULL_POS) {
        // Nothing to add
        if (pData == nullptr) {
          return *this;
        }

        // Calculate length of the array
        if (iSize == NULL_POS) {
          iSize = strlen(pData);
        }

        if (iPos > Size()) {
          iPos = Size();
        }

        // Allocate new buffer
        size_t iNewSize = Size() + iSize;
        c8 *pNewData = new c8[iNewSize + 1]; // Size + terminator
        pNewData[iNewSize] = '\0';

        // Copy first part of the source array
        if (iPos != 0) {
          memcpy(&pNewData[0], Data(), iPos);
        }

        memcpy(&pNewData[iPos], pData, iSize); // Copy data from another array
        memcpy(&pNewData[iPos + iSize], &Data()[iPos], Size() - iPos); // Copy second part of the source array

        // Delete old buffer
        Clear();

        _pBuffer = pNewData;
        _iSize = iNewSize;

        return *this;
      };

      // Insert specified amount of specified chars into specified position
      CByteArray &Insert(size_t iPos, size_t iCount, c8 chByte) {
        // Nothing to add
        if (iCount == 0) {
          return *this;
        }

        if (iPos > Size()) {
          iPos = Size();
        }

        // Allocate new buffer
        size_t iNewSize = Size() + iCount;
        c8 *pNewData = new c8[iNewSize + 1]; // Size + terminator
        pNewData[iNewSize] = '\0';

        // Copy first part of source array
        if (iPos != 0) {
          memcpy(&pNewData[0], Data(), iPos);
        }

        memset(&pNewData[iPos], chByte, iCount); // Fill with bytes
        memcpy(&pNewData[iPos + iCount], &Data()[iPos], Size() - iPos); // Copy second part of the source array

        // Delete old buffer
        Clear();

        _pBuffer = pNewData;
        _iSize = iNewSize;

        return *this;
      };

      // Remove specified amount of bytes from a certain position
      CByteArray &Remove(size_t iPos, size_t iSize) {
        if (IsNull() || iSize == 0 || iPos >= Size()) {
          return *this;
        }

        size_t iMaxRemoval = Size() - iPos;

        if (iSize > iMaxRemoval) {
          iSize = iMaxRemoval;
        }

        size_t iNewSize = Size() - iSize;
        size_t iRightSize = Size() - iSize - iPos;

        if (iRightSize != 0) {
          memcpy(&Data()[iPos], &Data()[iPos + iSize], iRightSize);
        }

        Resize(iNewSize);
        return *this;
      };

      // Change size of allocated memory
      void Resize(size_t iNewSize) {
        if (iNewSize == 0) {
          Clear();
          return;
        }

        c8 *pNewData = new c8[iNewSize + 1]; // Size + terminator
        pNewData[iNewSize] = '\0';

        if (Data()) {
          // Copy old data if enough space
          if (iNewSize > Size()) {
            if (_iSize != 0) {
              memcpy(&pNewData[0], _pBuffer, _iSize);
            }

          // Copy reduced data
          } else {
            if (iNewSize != 0) {
              memcpy(&pNewData[0], _pBuffer, iNewSize);
            }
          }

          Clear(); // Delete old data
        }

        // Set new buffer
        _pBuffer = pNewData;
        _iSize = iNewSize;
      };

      // Remove specified amound of bytes from the end
      void Chop(size_t iSize) {
        if (iSize > Size()) {
          iSize = Size();
        }

        Resize(Size() - iSize);
      };

      // Delete the array buffer
      void Clear(void) {
        if (_pBuffer != nullptr) {
          delete[] _pBuffer;

          _pBuffer = nullptr;
          _iSize = 0;
        }
      };

      // Return byte at a position
      c8 At(size_t i) const {
        D_ASSERT(_pBuffer != nullptr);
        D_ASSERT(i < _iSize);

        return _pBuffer[i];
      };

      // Fill array with a specific byte
      CByteArray &Fill(const c8 chByte, size_t iSize = NULL_POS) {
        if (Size() == 0 || IsNull()) {
          return *this;
        }

        if (iSize > Size()) {
          iSize = Size();
        }

        memset(_pBuffer, chByte, iSize);

        return *this;
      };

      // Return length of the byte array
      size_t Size(void) const {
        return _iSize;
      };

      // Check if memory for the array hasn't been allocated
      bool IsNull(void) const {
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
      CByteArray &operator=(const CByteArray &baOther) {
        Clear();

        if (baOther.Size() != 0) {
          Resize(baOther.Size());
          memcpy(Data(), baOther.ConstData(), baOther.Size());
        }

        return *this;
      };

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
      bool operator==(const CByteArray &baOther) const {
        // Different sizes 
        if (Size() != baOther.Size()) {
          return false;
        }

        // Both are empty
        if (IsNull() && baOther.IsNull()) {
          return true;
        }

        return memcmp(ConstData(), baOther.ConstData(), Size()) == 0;
      };

      // Equality comparison
      bool operator==(const c8 *strOther) const {
        // String is null and empty array
        if (IsNull() && strOther == nullptr) {
          return true;
        }

        // String is null but array is not empty
        if (strOther == nullptr) {
          return false;
        }

        size_t iSize = strlen(strOther);

        // Different sizes
        if (Size() != iSize) {
          return false;
        }

        return memcmp(ConstData(), strOther, Size()) == 0;
      };

      // Inequality comparison
      bool operator!=(const CByteArray &baOther) const {
        return !operator==(baOther);
      };

      // Inequality comparison
      bool operator!=(const c8 *strOther) const {
        return !operator==(strOther);
      };

      // Swap data with another byte array
      void Swap(CByteArray &baOther) {
        size_t iTempSize = Size();
        c8 *pTempData = Data();

        _iSize = baOther.Size();
        _pBuffer = baOther.Data();

        baOther._iSize = iTempSize;
        baOther._pBuffer = pTempData;
      };

      // Check if array starts with another byte array
      bool StartsWith(const CByteArray &baOther) const {
        if (baOther.IsNull() || baOther.Size() > Size()) {
          return false;
        }

        return memcmp(ConstData(), baOther.ConstData(), baOther.Size()) == 0;
      };

      // Check if array starts with a specified byte
      bool StartsWith(const c8 chByte) const {
        if (Size() == 0) {
          return false;
        }

        return Front() == chByte;
      };

      // Check if array starts with specified string
      bool StartsWith(const c8 *str) const {
        if (str == nullptr) {
          return false;
        }

        size_t iSize = strlen(str);

        if (iSize > Size()) {
          return false;
        }

        return memcmp(ConstData(), str, iSize) == 0;
      };

      // Check if array starts with another byte array
      bool EndsWith(const CByteArray &baOther) const {
        if (baOther.IsNull() || baOther.Size() > Size()) {
          return false;
        }

        size_t iPos = Size() - baOther.Size();
        return memcmp(&ConstData()[iPos], baOther.ConstData(), baOther.Size()) == 0;
      };

      // Check if array ends with a specified byte
      bool EndsWith(const c8 chByte) const {
        if (Size() == 0) {
          return false;
        }

        return Back() == chByte;
      };

      // Check if array ends with specified string
      bool EndsWith(const c8 *str) const {
        if (str == nullptr) {
          return false;
        }

        size_t iSize = strlen(str);

        if (iSize > Size()) {
          return false;
        }

        size_t iPos = Size() - iSize;
        return memcmp(&ConstData()[iPos], str, iSize) == 0;
      };

      // Return position of first occurence of byte array
      size_t IndexOf(const CByteArray &baOther, size_t iFrom = 0) {
        if (IsNull() || baOther.IsNull() || iFrom >= Size() || baOther.Size() > Size()) {
          return NULL_POS;
        }

        for (size_t i = 0; i < Size(); ++i) {
          size_t iLeft = Size() - i;

          if (iLeft < baOther.Size()) {
            return NULL_POS;
          }

          if (memcmp(&ConstData()[i], baOther.ConstData(), baOther.Size()) == 0) {
            return i;
          }
        }

        return NULL_POS;
      };

      // Return position of the first occurence of a byte
      size_t IndexOf(c8 chByte, size_t iFrom = 0) {
        if (IsNull() || iFrom >= Size()) {
          return NULL_POS;
        }

        for (size_t i = iFrom; i < Size(); ++i) {
          if (ConstData()[i] == chByte) {
            return i;
          }
        }

        return NULL_POS;
      };

      // Return position of first occurence of string
      size_t IndexOf(const c8 *str, size_t iFrom = 0) {
        if (IsNull() || str == nullptr || iFrom >= Size()) {
          return NULL_POS;
        }

        size_t iSize = strlen(str);

        if (iSize > Size()) {
          return NULL_POS;
        }

        for (size_t i = iFrom; i < Size(); ++i) {
          size_t iLeft = Size() - i;

          if (iLeft < iSize) {
            return NULL_POS;
          }

          if (memcmp(&ConstData()[i], str, iSize) == 0) {
            return i;
          }
        }

        return NULL_POS;
      };

      // Return HEX-encoded copy of the array
      CByteArray ToHex(const c8 chDelimiter = '\0') const {
        static const c8 *strDigits = "0123456789ABCDEF";

        size_t iLength = chDelimiter == '\0' ? Size() * 2 : (Size() * 3 - 1);

        CByteArray baResult('0', iLength);

        size_t iElement = 0;

        for (size_t i = 0; i < Size(); ++i) {
          baResult.Data()[iElement++] = strDigits[(ConstData()[i] & 0xF0) >> 4];
          baResult.Data()[iElement++] = strDigits[ConstData()[i] & 0x0F];

          if (chDelimiter != '\0' && (i + 1) < Size()) {
            baResult.Data()[iElement++] = chDelimiter;
          }
        }

        return baResult;
      };

    // Method aliases
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

};

#endif // (Dreamy Utilities Include Guard)
