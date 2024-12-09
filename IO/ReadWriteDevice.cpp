//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#include "ReadWriteDevice.hpp"

namespace dreamy {

IReadWriteDevice::~IReadWriteDevice() {};

size_t IReadWriteDevice::Read(CByteArray &baData, size_t iMaxSize) {
  baData.Clear();
  baData.Resize(iMaxSize);

  return Read(baData.Data(), iMaxSize);
};

size_t IReadWriteDevice::Peek(CByteArray &baData, size_t iMaxSize) {
  baData.Clear();
  baData.Resize(iMaxSize);

  return Peek(baData.Data(), iMaxSize);
};

size_t IReadWriteDevice::Write(const CByteArray &baData) {
  if (baData.IsNull()) return NULL_POS;

  return Write(baData.ConstData(), baData.Size());
};

}; // namespace dreamy
