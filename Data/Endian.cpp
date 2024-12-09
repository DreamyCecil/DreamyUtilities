//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#include "Endian.hpp"
#include "Memory.hpp"

namespace dreamy {

namespace endian {

u16 ToLittle(u16 iSrc)
{
  #if _DREAMY_BYTE_ORDER == _DREAMY_LITTLE_ENDIAN
    return iSrc;
  #else
    return ByteSwap16(iSrc);
  #endif
};

u32 ToLittle(u32 iSrc)
{
  #if _DREAMY_BYTE_ORDER == _DREAMY_LITTLE_ENDIAN
    return iSrc;
  #else
    return ByteSwap32(iSrc);
  #endif
};

u64 ToLittle(u64 iSrc)
{
  #if _DREAMY_BYTE_ORDER == _DREAMY_LITTLE_ENDIAN
    return iSrc;
  #else
    return ByteSwap64(iSrc);
  #endif
};

s16 ToLittle(s16 iSrc)
{
  #if _DREAMY_BYTE_ORDER == _DREAMY_LITTLE_ENDIAN
    return iSrc;
  #else
    return ByteSwap16(iSrc);
  #endif
};

s32 ToLittle(s32 iSrc)
{
  #if _DREAMY_BYTE_ORDER == _DREAMY_LITTLE_ENDIAN
    return iSrc;
  #else
    return ByteSwap32(iSrc);
  #endif
};

s64 ToLittle(s64 iSrc)
{
  #if _DREAMY_BYTE_ORDER == _DREAMY_LITTLE_ENDIAN
    return iSrc;
  #else
    return ByteSwap64(iSrc);
  #endif
};

f32 ToLittle(f32 fSrc)
{
  #if _DREAMY_BYTE_ORDER == _DREAMY_LITTLE_ENDIAN
    return fSrc;
  #else
    u32 *piSrc = reinterpret_cast<u32 *>(&fSrc);
    *piSrc = ByteSwap32(*piSrc);
    return fSrc;
  #endif
};

f64 ToLittle(f64 fSrc)
{
  #if _DREAMY_BYTE_ORDER == _DREAMY_LITTLE_ENDIAN
    return fSrc;
  #else
    u64 *piSrc = reinterpret_cast<u64 *>(&fSrc);
    *piSrc = ByteSwap64(*piSrc);
    return fSrc;
  #endif
};

#if _DREAMY_UNIX

size_t ToLittle(size_t iSrc)
{
  #if _DREAMY_BYTE_ORDER == _DREAMY_LITTLE_ENDIAN
    return iSrc;
  #else
    if (sizeof(size_t) == 8) {
      return ByteSwap64((u64)iSrc);
    } else {
      return ByteSwap32((u32)iSrc);
    }
  #endif
};

#endif

u16 ToBig(u16 iSrc)
{
  #if _DREAMY_BYTE_ORDER == _DREAMY_BIG_ENDIAN
    return iSrc;
  #else
    return ByteSwap16(iSrc);
  #endif
};

u32 ToBig(u32 iSrc)
{
  #if _DREAMY_BYTE_ORDER == _DREAMY_BIG_ENDIAN
    return iSrc;
  #else
    return ByteSwap32(iSrc);
  #endif
};

u64 ToBig(u64 iSrc)
{
  #if _DREAMY_BYTE_ORDER == _DREAMY_BIG_ENDIAN
    return iSrc;
  #else
    return ByteSwap64(iSrc);
  #endif
};

s16 ToBig(s16 iSrc)
{
  #if _DREAMY_BYTE_ORDER == _DREAMY_BIG_ENDIAN
    return iSrc;
  #else
    return ByteSwap16(iSrc);
  #endif
};

s32 ToBig(s32 iSrc)
{
  #if _DREAMY_BYTE_ORDER == _DREAMY_BIG_ENDIAN
    return iSrc;
  #else
    return ByteSwap32(iSrc);
  #endif
};

s64 ToBig(s64 iSrc)
{
  #if _DREAMY_BYTE_ORDER == _DREAMY_BIG_ENDIAN
    return iSrc;
  #else
    return ByteSwap64(iSrc);
  #endif
};

f32 ToBig(f32 fSrc)
{
  #if _DREAMY_BYTE_ORDER == _DREAMY_BIG_ENDIAN
    return fSrc;
  #else
    u32 *piSrc = reinterpret_cast<u32 *>(&fSrc);
    *piSrc = ByteSwap32(*piSrc);
    return fSrc;
  #endif
};

f64 ToBig(f64 fSrc)
{
  #if _DREAMY_BYTE_ORDER == _DREAMY_BIG_ENDIAN
    return fSrc;
  #else
    u64 *piSrc = reinterpret_cast<u64 *>(&fSrc);
    *piSrc = ByteSwap64(*piSrc);
    return fSrc;
  #endif
};

#if _DREAMY_UNIX

size_t ToBig(size_t iSrc)
{
  #if _DREAMY_BYTE_ORDER == _DREAMY_BIG_ENDIAN
    return iSrc;
  #else
    if (sizeof(size_t) == 8) {
      return ByteSwap64((u64)iSrc);
    } else {
      return ByteSwap32((u32)iSrc);
    }
  #endif
};

#endif

}; // namespace endian

}; // namespace dreamy
