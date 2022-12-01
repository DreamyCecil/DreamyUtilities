//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_FILEDEVICE_H
#define _DREAMYUTILITIES_INCL_FILEDEVICE_H

#include "../Base/Base.hpp"

#include "ReadWriteDevice.hpp"

#include "../Files/Filenames.hpp"

namespace dreamy
{
  class CFileDevice : public IReadWriteDevice {
    protected:
      FILE *_pFile;
      CPath _strFilename;

    public:
      // Default constructor
      CFileDevice() : _pFile(nullptr), _strFilename("")
      {
        _eOpenMode = OM_UNOPEN;
      };

      // Constructor with path to the file
      CFileDevice(const c8 *strPath) : _pFile(nullptr), _strFilename(strPath)
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

        if (_pFile != nullptr) {
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

      // Check if the carret at the end
      virtual bool AtEnd(void) const {
        s32 iEOF = feof(_pFile);
        return iEOF != 0;
      };

      // Return current carret position
      virtual size_t Pos(void) const {
        return (IsOpen() ? ftell(_pFile) : NULL_POS);
      };

      // Length of the file
      virtual size_t Size(void) const {
        if (!IsOpen()) return NULL_POS;

        s32 iCurrentPos = (s32)ftell(_pFile);
        fseek(_pFile, 0, SEEK_END);

        s32 iFileSize = (s32)ftell(_pFile);
        fseek(_pFile, iCurrentPos, SEEK_SET);

        return iFileSize;
      };

      // Try to move the carret to a specified position
      virtual bool Seek(size_t iOffset) {
        D_ASSERT(_pFile != nullptr);
        return fseek(_pFile, (s32)iOffset, SEEK_SET) == 0;
      };

      // Move forward
      virtual size_t Skip(size_t iMaxSize) {
        D_ASSERT(_pFile != nullptr);
        return (size_t)fseek(_pFile, (s32)iMaxSize, SEEK_CUR);
      };

      // Read bytes from the file
      virtual size_t Read(c8 *pData, size_t iMaxSize) {
        D_ASSERT(_pFile != nullptr);
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
        
        D_ASSERT(_pFile != nullptr);
        return fwrite(pData, 1, iMaxSize, _pFile);
      };

      // Return type of the device class
      virtual EDeviceType GetType(void) const {
        return TYPE_FILE;
      };

    // File manipulation
    public:

      // Return current filename
      Str_t GetFilename(void) const {
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

      // Return pointer to native file object
      inline FILE *GetFileObject(void) {
        return _pFile;
      };
  };

};

#endif // (Dreamy Utilities Include Guard)
