//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_FILEDEVICE_H
#define _DREAMYUTILITIES_INCL_FILEDEVICE_H

#include "../Base/Base.hpp"

#include "ReadWriteDevice.hpp"

#include "../Files/Filenames.hpp"

namespace dreamy {

class CFileDevice : public IReadWriteDevice {

protected:
  FILE *_pFile;
  size_t _iSize;
  CPath _strFilename;

public:
  // Default constructor
  CFileDevice() : _pFile(nullptr), _iSize(NULL_POS), _strFilename("")
  {
    _eOpenMode = OM_UNOPEN;
  };

  // Constructor with path to the file
  CFileDevice(const c8 *strPath) : _pFile(nullptr), _iSize(NULL_POS), _strFilename(strPath)
  {
    _eOpenMode = OM_UNOPEN;
  };

  // Destructor
  virtual ~CFileDevice() {
    Close();
  };

  // Set new path to the file
  virtual bool SetFilename(const c8 *strPath) {
    if (IsOpen()) return false;

    _strFilename = strPath;
    return true;
  };

  // Start interacting in a given mode
  virtual bool Open(EOpenMode eOpenMode) {
    if (eOpenMode == OM_UNOPEN) return false;

    c8 strOpenMode[4];

    switch (eOpenMode) {
      case OM_READONLY:  strcpy(strOpenMode, "rb"); break;
      case OM_WRITEONLY: strcpy(strOpenMode, "wb"); break;
      default: strcpy(strOpenMode, "rb+"); break;
    }

    FileOpen(&_pFile, _strFilename.c_str(), strOpenMode);

    if (_pFile != nullptr)
    {
      // Determine file size from the end position
      fseek(_pFile, 0, SEEK_END);
      _iSize = (size_t)ftell(_pFile);
      fseek(_pFile, 0, SEEK_SET);

      _eOpenMode = eOpenMode;
      return true;
    }

    return false;
  };

  // End interacting with
  virtual void Close(void) {
    if (!IsOpen()) return;

    fclose(_pFile);
    _eOpenMode = OM_UNOPEN;
  };

  // Check if the carret is at the end
  virtual bool AtEnd(void) const {
    return Pos() >= Size();
  };

  // Return current carret position
  virtual size_t Pos(void) const {
    return (IsOpen() ? ftell(_pFile) : NULL_POS);
  };

  // Length of the file
  virtual size_t Size(void) const {
    return (IsOpen() ? _iSize : NULL_POS);
  };

  // Try to move the carret to a specified position
  virtual bool Seek(size_t iOffset) {
    if (!IsOpen()) return false;
    return fseek(_pFile, (s32)iOffset, SEEK_SET) == 0;
  };

  // Move forward
  virtual size_t Skip(size_t iMaxSize) {
    if (!IsOpen()) return NULL_POS;

    size_t iLastPos = (size_t)ftell(_pFile);

    if (fseek(_pFile, (s32)iMaxSize, SEEK_CUR) == 0) {
      // Results in less than iMaxSize if limited by size
      size_t iCurPos = dreamy::math::Min((size_t)ftell(_pFile), Size());
      return (iCurPos - iLastPos);
    }

    return NULL_POS;
  };

  // Read bytes from the file
  virtual size_t Read(c8 *pData, size_t iMaxSize) {
    if (!IsReadable()) return NULL_POS;
    return fread(pData, 1, iMaxSize, _pFile);
  };

  // Read bytes without moving the carret forward
  virtual size_t Peek(c8 *pData, size_t iMaxSize) {
    size_t iCurrentPos = Pos();
    size_t iResult = Read(pData, iMaxSize);
    Seek(iCurrentPos);

    return iResult;
  };

  // Write bytes into the file
  virtual size_t Write(const c8 *pData, size_t iMaxSize) {
    if (!IsWritable()) return NULL_POS;
    return fwrite(pData, 1, iMaxSize, _pFile);
  };

  // Return type of the device class
  virtual EDeviceType GetType(void) const {
    return TYPE_FILE;
  };

// File manipulation
public:

  // Return pointer to native file object
  inline FILE *GetFileObject(void) {
    return _pFile;
  };

  // Return current filename
  inline const CPath &GetFilename(void) const {
    return _strFilename;
  };

  // Check if file exists on disk
  inline bool Exists(void) const {
    return FileExists(_strFilename.c_str());
  };

  // Try to remove the file from disk
  bool Remove(void) {
    s32 iResult = remove(_strFilename.c_str());
    return (iResult == 0);
  };

  // Try to rename file on disk
  bool Rename(const Str_t &strName) {
    if (_strFilename.length() == 0 && strName.length() == 0) {
      return false;
    }

    s32 iResult = rename(_strFilename.c_str(), strName.c_str());
    return (iResult == 0);
  };
};

};

#endif // (Dreamy Utilities Include Guard)
