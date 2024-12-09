//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_FILEDEVICE_H
#define _DREAMYUTILITIES_INCL_FILEDEVICE_H

#include "../DreamyUtilitiesBase.hpp"

#include "ReadWriteDevice.hpp"
#include "../Types/String.hpp"

namespace dreamy {

class CFileDevice : public IReadWriteDevice {

protected:
  FILE *_pFile;
  size_t _iSize;
  CString _strFilename;

public:
  // Default constructor
  CFileDevice();

  // Constructor with path to the file
  CFileDevice(const c8 *strPath);

  // Destructor
  virtual ~CFileDevice();

  // Set new path to the file
  virtual bool SetFilename(const c8 *strPath);

  // Start interacting in a given mode
  virtual bool Open(EOpenMode eOpenMode);

  // End interacting with
  virtual void Close(void);

  // Check if the carret is at the end
  virtual bool AtEnd(void) const;

  // Return current carret position
  virtual size_t Pos(void) const;

  // Length of the file
  virtual size_t Size(void) const;

  // Try to move the carret to a specified position
  virtual bool Seek(size_t iOffset);

  // Move forward
  virtual size_t Skip(size_t iMaxSize);

  // Read bytes from the file
  virtual size_t Read(c8 *pData, size_t iMaxSize);

  // Read bytes without moving the carret forward
  virtual size_t Peek(c8 *pData, size_t iMaxSize);

  // Write bytes into the file
  virtual size_t Write(const c8 *pData, size_t iMaxSize);

  // Return type of the device class
  virtual EDeviceType GetType(void) const {
    return TYPE_FILE;
  };

// File manipulation
public:

  // Return pointer to native file object
  inline FILE *GetFileObject(void);

  // Return current filename
  inline const CString &GetFilename(void) const;

  // Check if file exists on disk
  inline bool Exists(void) const;

  // Try to remove the file from disk
  bool Remove(void);

  // Try to rename file on disk
  bool Rename(const CString &strName);
};

}; // namespace dreamy

#endif // (Dreamy Utilities Include Guard)
