//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_READWRITEDEVICE_H
#define _DREAMYUTILITIES_INCL_READWRITEDEVICE_H

#include "../DreamyUtilitiesBase.hpp"

#include "../Types/ByteArray.hpp"

namespace dreamy {

// Abstract interface for random-access device
class IReadWriteDevice {

public:
  // Access modes that the device can operate in
  enum EOpenMode {
    OM_UNOPEN    = 0, // Closed
    OM_READONLY  = (1 << 0), // "rb"
    OM_WRITEONLY = (1 << 1), // "wb"
    OM_READWRITE = OM_READONLY | OM_WRITEONLY, // "rb+"
  };

  // Distinctive type of the device class
  enum EDeviceType {
    TYPE_INVALID = 0,
    TYPE_BUFFER,
    TYPE_FILE,
    TYPE_LOCALSOCKET,
  };

protected:
  EOpenMode _eOpenMode; // Which access mode the device is currently in

public:
  // Destructor
  virtual ~IReadWriteDevice();

  // Start interacting in a given mode
  virtual bool Open(EOpenMode eOpenMode) = 0;

  // End interacting with
  virtual void Close() = 0;

  // Return access mode
  inline EOpenMode GetOpenMode(void) const {
    return _eOpenMode;
  };

  // Checks if opened with any mode
  inline bool IsOpen(void) const {
    return (GetOpenMode() != OM_UNOPEN);
  };

  // Check if the device can be read from
  inline bool IsReadable(void) const {
    return (GetOpenMode() & OM_READONLY) != 0;
  };

  // Check if the device can be written in
  inline bool IsWritable(void) const {
    return (GetOpenMode() & OM_WRITEONLY) != 0;
  };

  // Check if the carret is at the end
  virtual bool AtEnd(void) const = 0;

  // Return current carret position
  virtual size_t Pos(void) const = 0;

  // Length of the device
  virtual size_t Size(void) const = 0;

  // Seek to the beginning
  virtual bool Reset(void) {
    return Seek(0);
  };

  // Try to move carret to a specified position
  virtual bool Seek(size_t iOffset) = 0;

  // Move forward
  virtual size_t Skip(size_t iMaxSize) = 0;

  // Take bytes from the device
  virtual size_t Read(c8 *pData, size_t iMaxSize) = 0;

  // Take bytes from the device
  virtual size_t Read(CByteArray &baData, size_t iMaxSize);

  // Take bytes without moving the carret forward
  virtual size_t Peek(c8 *pData, size_t iMaxSize) = 0;

  // Take bytes without moving the carret forward
  virtual size_t Peek(CByteArray &baData, size_t iMaxSize);

  // Put bytes into the device
  virtual size_t Write(const c8 *pData, size_t iMaxSize) = 0;

  // Put bytes into the device
  virtual size_t Write(const CByteArray &baData);

  // Get type of the device class
  virtual EDeviceType GetType(void) const {
    return TYPE_INVALID;
  };
};

};

#endif // (Dreamy Utilities Include Guard)
