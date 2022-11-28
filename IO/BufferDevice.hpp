// This file is a part of Dreamy Utilities

#ifndef _DREAMYUTILITIES_INCL_BUFFERDEVICE_H
#define _DREAMYUTILITIES_INCL_BUFFERDEVICE_H

#include "../Base/Base.hpp"

#include "../Data/ByteArray.hpp"
#include "ReadWriteDevice.hpp"

namespace dreamy
{
  class CBufferDevice : public IReadWriteDevice {
    protected:
      CByteArray *_pData;
      size_t _iPos;

    public:
      // Default constructor
      CBufferDevice() : _pData(nullptr), _iPos(0)
      {
      };

      // Constructor that uses pointed CByteArray as internal buffer
      CBufferDevice(CByteArray *pByteArray) : _pData(pByteArray), _iPos(0)
      {
      };

      // Start interacting in a given mode
      virtual bool Open(EOpenMode eOpenMode) {
        if (_pData == nullptr) {
          return false;
        }

        _eOpenMode = eOpenMode;
        _iPos = 0;
        return IsOpen();
      };

      // End interacting with
      virtual void Close(void) {
        _eOpenMode = OM_UNOPEN;
        _iPos = 0;
      };

      // Return current carret position
      virtual size_t Pos(void) const {
        return IsOpen() ? _iPos : NULL_POS;
      };

      // Check if the carret at the end
      virtual bool AtEnd(void) const {
        return Pos() >= Size();
      };

      // Length of the buffer
      virtual size_t Size(void) const {
        return _pData->Size();
      };

      // Try to move the carret to a specified position
      virtual bool Seek(size_t iOffset) {
        if (_pData == nullptr || IsOpen()) {
          return false;
        }

        // Past the limit
        if (iOffset >= Size()) return false;

        // Set new position
        _iPos = iOffset;
        return true;
      };

      // Move forward
      virtual size_t Skip(size_t iMaxSize) {
        _iPos += iMaxSize;
        return iMaxSize;
      };

      // Take bytes from the device
      virtual size_t Read(c8 *pData, size_t iMaxSize) {
        if (pData == nullptr || _pData == nullptr || _pData->IsNull()) {
          return NULL_POS;
        }

        if (AtEnd()) return 0;

        size_t iExpectedPos = _iPos + iMaxSize;

        // Past the limit
        if (iExpectedPos > Size()) {
          size_t iNotEnough = iExpectedPos - Size();
          iMaxSize -= iNotEnough;
        }

        memcpy(pData, &_pData->ConstData()[_iPos], iMaxSize);
        _iPos += iMaxSize;

        return iMaxSize;
      };

      // Take bytes without moving carret forward
      virtual size_t Peek(c8 *pData, size_t iMaxSize) {
        size_t iCurrentPos = Pos();
        size_t iLength = Read(pData, iMaxSize);
        Seek(iCurrentPos);

        return iLength;
      };

      // Put bytes into the device
      virtual size_t Write(const c8 *pData, size_t iMaxSize) {
        if (pData == NULL || _pData == nullptr || !IsWritable()) {
          return NULL_POS;
        }

        if (_iPos + iMaxSize >= Size()) {
          _pData->Resize(Size() + iMaxSize);
        }

        memcpy(&_pData->Data()[_iPos], pData, iMaxSize);
        _iPos += iMaxSize;

        return iMaxSize;
      };

      // Get type of the device class
      virtual EDeviceType GetType(void) const {
        return TYPE_BUFFER;
      };

    // Buffer manipulation
    public:

      // Set new byte array (while unopen)
      void SetBuffer(CByteArray *pData) {
        if (IsOpen()) return;
        _pData = pData;
      };

      // Get byte array
      const c8 *GetBuffer(void) const {
        D_ASSERT(_pData != nullptr);
        return _pData->ConstData();
      };
  };

};

#endif // (Dreamy Utilities Include Guard)
