//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_DATASTREAM_H
#define _DREAMYUTILITIES_INCL_DATASTREAM_H

#include "../DreamyUtilitiesBase.hpp"

#include "ReadWriteDevice.hpp"
#include "../Types/String.hpp"
#include "../Types/ByteArray.hpp"

namespace dreamy {

class CDataStream {

public:
  // Byte order that the stream can operate in
  enum EByteOrder {
    BO_BIGENDIAN    = _DREAMY_BIG_ENDIAN,
    BO_LITTLEENDIAN = _DREAMY_LITTLE_ENDIAN,

    // Platform-specific byte order
    BO_PLATFORM = _DREAMY_BYTE_ORDER,
  };

  // Stream status types
  enum EStatus {
    STATUS_OK, // No error
    STATUS_READPASTEND, // Failed to read - went past the stream size
    STATUS_WRITEFAILED, // Failed to write
  };

protected:
  IReadWriteDevice *_pDevice; // Device that's being used for serialization
  EByteOrder _eByteOrder;     // Current byte order for writing and reading
  EStatus _eStatus;           // Current stream status
  bool _bExceptionMode;       // Throw exceptions on stream errors
  bool _bHasOwnDevice;        // Stream owns the pointer to the device

public:
  // Default constructor
  CDataStream();

  // Constructor from a device
  CDataStream(IReadWriteDevice *d);

  // Constructor from a read-only byte array
  CDataStream(const CByteArray &ba);

  // Constructor from a byte array
  CDataStream(CByteArray *pba, IReadWriteDevice::EOpenMode om);

  // Destructor
  virtual ~CDataStream();

  // Returns current byte order used for writing and reading
  inline EByteOrder GetByteOrder(void) const {
    return _eByteOrder;
  };

  // Change byte order used for writing and reading
  void SetByteOrder(EByteOrder bo) {
    _eByteOrder = bo;
  };

  // Wrapper for IReadWriteDevice::AtEnd()
  bool AtEnd(void) const;

  // Return used device
  inline IReadWriteDevice *Device(void) {
    return _pDevice;
  };

  // Return current status
  inline EStatus GetStatus(void) {
    return _eStatus;
  };

  // Change active device
  void SetDevice(IReadWriteDevice *d);

  // Change status only if the current status is 'STATUS_OK'
  void SetStatus(const EStatus eStatus);

  // Toggle the exception mode
  void SetExceptionMode(bool bState) {
    _bExceptionMode = bState;
  };

  // Change status to STATUS_OK
  void ResetStatus(void) {
    _eStatus = STATUS_OK;
  };

  // Read from the device
  size_t Read(void *pBuffer, size_t iLength);

  // Read from the device
  CByteArray Read(size_t iLength);

  // Write into the device
  size_t Write(const void *pData, size_t iLength);

  // Write into the device
  size_t Write(const CByteArray &baData);

  // Return current carret position
  size_t Pos(void) const;

  // Try to move the carret to a specified position
  bool Seek(size_t iOffset);

  // Move forward
  size_t Skip(size_t iLength);

  // Read without moving the carret forward
  size_t Peek(void *pBuffer, size_t iLength);

  // Read without moving the carret forward
  CByteArray Peek(size_t iLength);

  // Read expected bytes
  bool Expect(const CByteArray &baData);

  // Read methods
  virtual CDataStream &operator>>(u8 &dst);
  virtual CDataStream &operator>>(u16 &dst);
  virtual CDataStream &operator>>(u32 &dst);
  virtual CDataStream &operator>>(u64 &dst);
  virtual CDataStream &operator>>(s8 &dst);
  virtual CDataStream &operator>>(s16 &dst);
  virtual CDataStream &operator>>(s32 &dst);
  virtual CDataStream &operator>>(s64 &dst);
  virtual CDataStream &operator>>(f32 &dst);
  virtual CDataStream &operator>>(f64 &dst);

  // Write methods
  virtual CDataStream &operator<<(u8 src);
  virtual CDataStream &operator<<(u16 src);
  virtual CDataStream &operator<<(u32 src);
  virtual CDataStream &operator<<(u64 src);
  virtual CDataStream &operator<<(s8 src);
  virtual CDataStream &operator<<(s16 src);
  virtual CDataStream &operator<<(s32 src);
  virtual CDataStream &operator<<(s64 src);
  virtual CDataStream &operator<<(f32 src);
  virtual CDataStream &operator<<(f64 src);

  // size_t is not the same as u32/u64 in Unix
  #if _DREAMY_UNIX
  virtual CDataStream &operator<<(size_t src);
  virtual CDataStream &operator>>(size_t &dst);
  #endif

  virtual CDataStream &operator<<(c8 src);
  virtual CDataStream &operator>>(c8 &dst);
  virtual CDataStream &operator<<(const CByteArray &baData);
  virtual CDataStream &operator>>(CByteArray &baData);
  virtual CDataStream &operator<<(const CString &str);
  virtual CDataStream &operator>>(CString &str);
  virtual CDataStream &operator<<(const c8 *str);
  virtual CDataStream &operator>>(c8 *str);
};

}; // namespace dreamy

#endif // (Dreamy Utilities Include Guard)
