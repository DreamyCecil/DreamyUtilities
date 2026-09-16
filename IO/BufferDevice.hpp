/* Copyright (c) 2022-2026 Dreamy Cecil
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DREAMYUTILITIES_INCL_BUFFERDEVICE_H
#define _DREAMYUTILITIES_INCL_BUFFERDEVICE_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

#include "ReadWriteDevice.hpp"

NAMESPACE_DREAMY_OPEN

class CBufferDevice : public IReadWriteDevice {

protected:
  CByteArray *m_pData;
  size_t m_iPos;

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

NAMESPACE_DREAMY_CLOSE

#endif // (Dreamy Utilities Include Guard)
