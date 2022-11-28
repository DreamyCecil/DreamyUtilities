// This file is a part of Dreamy Utilities

#ifndef _DREAMYUTILITIES_INCL_DATASTREAM_H
#define _DREAMYUTILITIES_INCL_DATASTREAM_H

#include "../Base/Base.hpp"

#include "../Data/Endian.hpp"
#include "../Types/Exception.hpp"

#include "../Data/ByteArray.hpp"
#include "ReadWriteDevice.hpp"
#include "BufferDevice.hpp"

namespace dreamy
{
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
      CDataStream() : _pDevice(nullptr),
        _eByteOrder(BO_PLATFORM), _eStatus(STATUS_OK),
        _bExceptionMode(false), _bHasOwnDevice(false)
      {
      };

      // Constructor from a device
      CDataStream(IReadWriteDevice *d) : _pDevice(d),
        _eByteOrder(BO_PLATFORM), _eStatus(STATUS_OK),
        _bExceptionMode(false), _bHasOwnDevice(false)
      {
      };

      // Constructor from a read-only byte array
      CDataStream(const CByteArray &ba) :
        _eByteOrder(BO_PLATFORM), _eStatus(STATUS_OK),
        _bExceptionMode(false), _bHasOwnDevice(true)
      {
        _pDevice = new CBufferDevice(const_cast<CByteArray *>(&ba));
        _pDevice->Open(IReadWriteDevice::OM_READONLY);
      };

      // Constructor from a byte array
      CDataStream(CByteArray *pba, IReadWriteDevice::EOpenMode om) :
        _eByteOrder(BO_PLATFORM), _eStatus(STATUS_OK),
        _bExceptionMode(false), _bHasOwnDevice(true)
      {
        _pDevice = new CBufferDevice(pba);
        _pDevice->Open(om);
      };

      // Destructor
      virtual ~CDataStream()
      {
        if (_bHasOwnDevice) {
          delete _pDevice;
        }
      };

      // Returns current byte order used for writing and reading
      inline EByteOrder GetByteOrder(void) const {
        return _eByteOrder;
      };

      // Change byte order used for writing and reading
      void SetByteOrder(EByteOrder bo) {
        _eByteOrder = bo;
      };

      // Wrapper for IReadWriteDevice::AtEnd()
      bool AtEnd(void) const {
        return (_pDevice == nullptr || _pDevice->AtEnd());
      };

      // Return used device
      inline IReadWriteDevice *Device(void) {
        return _pDevice;
      };

      // Return current status
      inline EStatus GetStatus(void) {
        return _eStatus;
      };

      // Change active device
      void SetDevice(IReadWriteDevice *d) {
        if (_bHasOwnDevice) {
          delete _pDevice;
          _bHasOwnDevice = false;
        }

        _pDevice = d;
      };

      // Change status only if the current status is 'STATUS_OK'
      void SetStatus(const EStatus eStatus) {
        // Not OK
        if (eStatus != STATUS_OK) return;

        _eStatus = eStatus;

        // Don't throw exceptions
        if (!_bExceptionMode) return;

        switch (eStatus) {
          case STATUS_READPASTEND: throw CMessageException("STATUS_READPASTEND"); break;
          case STATUS_WRITEFAILED: throw CMessageException("STATUS_WRITEFAILED"); break;
          default: break;
        }
      };

      // Toggle the exception mode
      void SetExceptionMode(bool bState) {
        _bExceptionMode = bState;
      };

      // Change status to STATUS_OK
      void ResetStatus(void) {
        _eStatus = STATUS_OK;
      };

      // Read from the device
      size_t Read(void *pBuffer, size_t iLength) {
        if (_eStatus != STATUS_OK || pBuffer == nullptr || Device() == nullptr) {
          return NULL_POS;
        }

        const size_t iResult = Device()->Read((c8 *)pBuffer, iLength);

        if (iResult != iLength) {
          SetStatus(STATUS_READPASTEND);
        }

        return iResult;
      };

      // Read from the device
      CByteArray Read(size_t iLength) {
        CByteArray baResult;

        baResult.Resize(iLength);
        Read(baResult.Data(), iLength);

        return baResult;
      };

      // Write into the device
      size_t Write(const void *pData, size_t iLength) {
        if (_eStatus != STATUS_OK || pData == nullptr || Device() == nullptr) {
          return NULL_POS;
        }

        return Device()->Write((const c8 *)pData, iLength);
      };

      // Write into the device
      size_t Write(const CByteArray &baData) {
        if (baData.IsNull()) return NULL_POS;

        return Write(baData.ConstData(), baData.Size());
      };

      // Return current carret position
      size_t Pos(void) const {
        return _pDevice->Pos();
      };

      // Try to move the carret to a specified position
      bool Seek(size_t iOffset) {
        if (_eStatus != STATUS_OK) return false;

        bool bResult = Device()->Seek(iOffset);

        if (!bResult) {
          SetStatus(STATUS_READPASTEND);
        }

        return bResult;
      };

      // Move forward
      size_t Skip(size_t iLength) {
        if (_eStatus != STATUS_OK) return NULL_POS;

        const size_t iResult = Device()->Skip(iLength);

        if (iResult != iLength) {
          SetStatus(STATUS_READPASTEND);
        }

        return iResult;
      };

      // Read without moving the carret forward
      size_t Peek(void *pBuffer, size_t iLength) {
        if (_eStatus != STATUS_OK) return NULL_POS;

        const size_t iResult = Device()->Peek((c8 *)pBuffer, iLength);

        if (iResult != iLength) {
          SetStatus(STATUS_READPASTEND);
        }

        return iResult;
      };

      // Read without moving the carret forward
      CByteArray Peek(size_t iLength) {
        CByteArray baResult;

        baResult.Resize(iLength);
        Peek(baResult.Data(), iLength);

        return baResult;
      };

      // Read expected bytes
      bool Expect(const CByteArray &baData) {
        CByteArray baResult = Read(baData.Size());
        
        // Mismatching bytes
        if (baResult != baData) {
          CMessageException::Throw("Expected '%s' sequence of bytes at 0x%X but got '%s'", baData.ConstData(), Pos(), baResult.ConstData());
          return false;
        }

        return true;
      };

      // Change byte order of the value before writing/after reading
      #define CHANGE_BYTE_ORDER(_Value) \
        if (GetByteOrder() == BO_LITTLEENDIAN) { \
          _Value = endian::ToLittle(_Value); \
        } else { \
          _Value = endian::ToBig(_Value); \
        }

      // Read methods

      virtual CDataStream &operator>>(u8 &dst) {
        if (Read(&dst, 1) != 1) dst = 0;
        return *this;
      };

      virtual CDataStream &operator>>(u16 &dst) {
        if (Read(&dst, 2) != 2) dst = 0;
        CHANGE_BYTE_ORDER(dst);
        return *this;
      };

      virtual CDataStream &operator>>(u32 &dst) {
        if (Read(&dst, 4) != 4) dst = 0;
        CHANGE_BYTE_ORDER(dst);
        return *this;
      };

      virtual CDataStream &operator>>(u64 &dst) {
        if (Read(&dst, 8) != 8) dst = 0;
        CHANGE_BYTE_ORDER(dst);
        return *this;
      };

      virtual CDataStream &operator>>(s8 &dst) {
        if (Read(&dst, 1) != 1) dst = 0;
        return *this;
      };

      virtual CDataStream &operator>>(s16 &dst) {
        if (Read(&dst, 2) != 2) dst = 0;
        CHANGE_BYTE_ORDER(dst);
        return *this;
      };

      virtual CDataStream &operator>>(s32 &dst) {
        if (Read(&dst, 4) != 4) dst = 0;
        CHANGE_BYTE_ORDER(dst);
        return *this;
      };

      virtual CDataStream &operator>>(s64 &dst) {
        if (Read(&dst, 8) != 8) dst = 0;
        CHANGE_BYTE_ORDER(dst);
        return *this;
      };

      virtual CDataStream &operator>>(f32 &dst) {
        if (Read(&dst, 4) != 4) dst = 0;
        CHANGE_BYTE_ORDER(dst);
        return *this;
      };

      virtual CDataStream &operator>>(f64 &dst) {
        if (Read(&dst, 8) != 8) dst = 0;
        CHANGE_BYTE_ORDER(dst);
        return *this;
      };

      // Write methods

      virtual CDataStream &operator<<(u8 src) { 
        if (Write(&src, 1) != 1) SetStatus(STATUS_WRITEFAILED);
        return *this;
      };

      virtual CDataStream &operator<<(u16 src) {
        CHANGE_BYTE_ORDER(src);
        if (Write(&src, 2) != 2) SetStatus(STATUS_WRITEFAILED);
        return *this;
      };

      virtual CDataStream &operator<<(u32 src) {
        CHANGE_BYTE_ORDER(src);
        if (Write(&src, 4) != 4) SetStatus(STATUS_WRITEFAILED);
        return *this;
      };

      virtual CDataStream &operator<<(u64 src) {
        CHANGE_BYTE_ORDER(src);
        if (Write(&src, 8) != 8) SetStatus(STATUS_WRITEFAILED);
        return *this;
      };

      virtual CDataStream &operator<<(s8 src) {
        if (Write(&src, 1) != 1) SetStatus(STATUS_WRITEFAILED);
        return *this;
      };

      virtual CDataStream &operator<<(s16 src) {
        CHANGE_BYTE_ORDER(src);
        if (Write(&src, 2) != 2) SetStatus(STATUS_WRITEFAILED);
        return *this;
      };

      virtual CDataStream &operator<<(s32 src) {
        CHANGE_BYTE_ORDER(src);
        if (Write(&src, 4) != 4) SetStatus(STATUS_WRITEFAILED);
        return *this;
      };

      virtual CDataStream &operator<<(s64 src) {
        CHANGE_BYTE_ORDER(src);
        if (Write(&src, 8) != 8) SetStatus(STATUS_WRITEFAILED);
        return *this;
      };

      virtual CDataStream &operator<<(f32 src) {
        CHANGE_BYTE_ORDER(src);
        if (Write(&src, 4) != 4) SetStatus(STATUS_WRITEFAILED);
        return *this;
      };

      virtual CDataStream &operator<<(f64 src) {
        CHANGE_BYTE_ORDER(src);
        if (Write(&src, 8) != 8) SetStatus(STATUS_WRITEFAILED);
        return *this;
      };

      // size_t is not the same as u32/u64 in Unix
      #if _DREAMY_UNIX

      virtual CDataStream &operator<<(size_t src) {
        CHANGE_BYTE_ORDER(src);
        if (Write(&src, sizeof(size_t)) != sizeof(size_t)) SetStatus(STATUS_WRITEFAILED);
        return *this;
      };

      virtual CDataStream &operator>>(size_t &dst) {
        if (Read(&dst, sizeof(size_t)) != sizeof(size_t)) dst = 0;
        CHANGE_BYTE_ORDER(dst);
        return *this;
      };

      #endif

      virtual CDataStream &operator<<(c8 src) {
        if (Write(&src, 1) != 1) SetStatus(STATUS_WRITEFAILED);
        return *this;
      };

      virtual CDataStream &operator>>(c8 &dst) {
        if (Read(&dst, 1) != 1) dst = 0;
        return *this;
      };

      virtual CDataStream &operator<<(const CByteArray &baData) {
        // Write length
        const u32 ulDataLength = (u32)baData.Size();
        *this << ulDataLength;

        // Write bytes if there are any
        if (ulDataLength > 0 && Write(baData.ConstData(), ulDataLength) != ulDataLength) {
          SetStatus(STATUS_WRITEFAILED);
        }

        return *this;
      };

      virtual CDataStream &operator>>(CByteArray &baData) {
        // Read length
        u32 ulDataLength;
        *this >> ulDataLength;

        baData.Clear();
        baData.Resize(ulDataLength);

        // Read bytes if there are any written
        if (ulDataLength > 0 && Read(baData.Data(), ulDataLength) != ulDataLength) {
          memset(baData.Data(), 0, ulDataLength);
        }

        return *this;
      };

      virtual CDataStream &operator<<(const Str_t &str) {
        // Write length
        size_t iSize = str.length();
        *this << (u32)iSize;

        // Write every character
        for (size_t i = 0; i < iSize; ++i) {
          *this << str[i];
        }

        return *this;
      };

      virtual CDataStream &operator>>(Str_t &str) {
        // Read length
        u32 iSize;
        *this >> iSize;

        // Read every character into the string
        str.resize(iSize);

        if (Read(&str[0], iSize) != iSize) {
          memset(&str[0], '\0', iSize);
        }

        return *this;
      };

      virtual CDataStream &operator<<(const c8 *str) {
        // Write length
        size_t iSize = strlen(str);
        *this << (u32)iSize;

        // Write every character
        for (size_t i = 0; i < iSize; ++i) {
          *this << str[i];
        }

        return *this;
      };

      virtual CDataStream &operator>>(c8 *str) {
        // Read length
        u32 iSize;
        *this >> iSize;

        // Read every character into the string
        if (Read(&str[0], iSize) != iSize) {
          memset(&str[0], '\0', iSize);
        }

        return *this;
      };
  };

};

#endif // (Dreamy Utilities Include Guard)
