//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#include "SimpleHasher.hpp"

namespace dreamy {

CSimpleHasher::CSimpleHasher(u64 iSetFactor) : _result(0), _factor(iSetFactor)
{
};

void CSimpleHasher::Reset(void) {
  _result = 0;
};

void CSimpleHasher::AddData(const c8 *pData, size_t iSize)
{
  for (size_t i = 0; i < iSize; ++i) {
    _result = _factor * _result + pData[i];
  }
};

CByteArray CSimpleHasher::GetBytes(void) const
{
  return CByteArray(reinterpret_cast<const c8 *>(&_result), sizeof(_result));
};

u64 CSimpleHasher::GetResult(void) const {
  return _result;
};

u64 CSimpleHasher::GetFactor(void) const {
  return _factor;
};

void CSimpleHasher::SetFactor(u64 iSetFactor) {
  _factor = iSetFactor;
};

u64 CSimpleHasher::operator()(const c8 *pData, size_t iSize) {
  Reset();
  AddData(pData, iSize);

  return _result;
};

};
