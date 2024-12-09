//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_BUFFERDEVICE_H
#define _DREAMYUTILITIES_INCL_BUFFERDEVICE_H

#include "../DreamyUtilitiesBase.hpp"

#include "ReadWriteDevice.hpp"

namespace dreamy {

class CBufferDevice : public IReadWriteDevice {

protected:
  CByteArray *_pData;
  size_t _iPos;

public:
  // Default constructor
  CBufferDevice();

  // Constructor that uses pointed CByteArray as internal buffer
  CBufferDevice(CByteArray *pByteArray);

  // Start interacting in a given mode
  virtual bool Open(EOpenMode eOpenMode);

  // End interacting with
  virtual void Close(void);

  // Return current carret position
  virtual size_t Pos(void) const;

  // Check if the carret is at the end
  virtual bool AtEnd(void) const;

  // Length of the buffer
  virtual size_t Size(void) const;

  // Try to move the carret to a specified position
  virtual bool Seek(size_t iOffset);

  // Move forward
  virtual size_t Skip(size_t iMaxSize);

  // Take bytes from the device
  virtual size_t Read(c8 *pData, size_t iMaxSize);

  // Take bytes without moving carret forward
  virtual size_t Peek(c8 *pData, size_t iMaxSize);

  // Put bytes into the device
  virtual size_t Write(const c8 *pData, size_t iMaxSize);

  // Get type of the device class
  virtual EDeviceType GetType(void) const {
    return TYPE_BUFFER;
  };

// Buffer manipulation
public:

  // Set new byte array (while unopen)
  void SetBuffer(CByteArray *pData);

  // Get byte array
  const c8 *GetBuffer(void) const;
};

}; // namespace dreamy

#endif // (Dreamy Utilities Include Guard)
