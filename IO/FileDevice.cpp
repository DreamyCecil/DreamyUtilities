//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#include "FileDevice.hpp"
#include "Files.hpp"
#include "../Math/Algorithm.hpp"

namespace dreamy {

// Default constructor
CFileDevice::CFileDevice() : _pFile(nullptr), _iSize(NULL_POS), _strFilename("")
{
  _eOpenMode = OM_UNOPEN;
};

// Constructor with path to the file
CFileDevice::CFileDevice(const c8 *strPath) : _pFile(nullptr), _iSize(NULL_POS), _strFilename(strPath)
{
  _eOpenMode = OM_UNOPEN;
};

// Destructor
CFileDevice::~CFileDevice() {
  Close();
};

// Set new path to the file
bool CFileDevice::SetFilename(const c8 *strPath) {
  if (IsOpen()) return false;

  _strFilename = strPath;
  return true;
};

bool CFileDevice::Open(EOpenMode eOpenMode) {
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

void CFileDevice::Close(void) {
  if (!IsOpen()) return;

  fclose(_pFile);
  _eOpenMode = OM_UNOPEN;
};

bool CFileDevice::AtEnd(void) const {
  return Pos() >= Size();
};

size_t CFileDevice::Pos(void) const {
  return (IsOpen() ? ftell(_pFile) : NULL_POS);
};

size_t CFileDevice::Size(void) const {
  return (IsOpen() ? _iSize : NULL_POS);
};

bool CFileDevice::Seek(size_t iOffset) {
  if (!IsOpen()) return false;
  return fseek(_pFile, (s32)iOffset, SEEK_SET) == 0;
};

size_t CFileDevice::Skip(size_t iMaxSize) {
  if (!IsOpen()) return NULL_POS;

  size_t iLastPos = (size_t)ftell(_pFile);

  if (fseek(_pFile, (s32)iMaxSize, SEEK_CUR) == 0) {
    // Results in less than iMaxSize if limited by size
    size_t iCurPos = dreamy::math::Min((size_t)ftell(_pFile), Size());
    return (iCurPos - iLastPos);
  }

  return NULL_POS;
};

size_t CFileDevice::Read(c8 *pData, size_t iMaxSize) {
  if (!IsReadable()) return NULL_POS;
  return fread(pData, 1, iMaxSize, _pFile);
};

size_t CFileDevice::Peek(c8 *pData, size_t iMaxSize) {
  size_t iCurrentPos = Pos();
  size_t iResult = Read(pData, iMaxSize);
  Seek(iCurrentPos);

  return iResult;
};

size_t CFileDevice::Write(const c8 *pData, size_t iMaxSize) {
  if (!IsWritable()) return NULL_POS;
  return fwrite(pData, 1, iMaxSize, _pFile);
};

FILE *CFileDevice::GetFileObject(void) {
  return _pFile;
};

const CString &CFileDevice::GetFilename(void) const {
  return _strFilename;
};

bool CFileDevice::Exists(void) const {
  return FileExists(_strFilename.c_str());
};

bool CFileDevice::Remove(void) {
  s32 iResult = remove(_strFilename.c_str());
  return (iResult == 0);
};

bool CFileDevice::Rename(const CString &strName) {
  if (_strFilename.length() == 0 && strName.length() == 0) {
    return false;
  }

  s32 iResult = rename(_strFilename.c_str(), strName.c_str());
  return (iResult == 0);
};

}; // namespace dreamy
