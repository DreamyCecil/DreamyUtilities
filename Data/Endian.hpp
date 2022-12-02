//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_ENDIAN_H
#define _DREAMYUTILITIES_INCL_ENDIAN_H

#include "../Base/Base.hpp"

// Byte-swapping functions
#include "ByteSwap.hpp"

namespace dreamy
{
  // Check if operating in little-endian at runtime
  __forceinline bool IsLittleEndian(void) {
    static const u32 i = 0x01020304;
    return *((u8 *)&i) == 0x04;
  };

  // Check if operating in big-endian at runtime
  __forceinline bool IsBigEndian(void) {
    static const u32 i = 0x01020304;
    return *((u8 *)&i) == 0x01;
  };

  // Converters between big and little endianness
  namespace endian
  {
    inline u16 ToLittle(u16 iSrc)
    {
      #if _DREAMY_BYTE_ORDER == _DREAMY_LITTLE_ENDIAN
        return iSrc;
      #else
        return ByteSwap16(iSrc);
      #endif
    };

    inline u32 ToLittle(u32 iSrc)
    {
      #if _DREAMY_BYTE_ORDER == _DREAMY_LITTLE_ENDIAN
        return iSrc;
      #else
        return ByteSwap32(iSrc);
      #endif
    };

    inline u64 ToLittle(u64 iSrc)
    {
      #if _DREAMY_BYTE_ORDER == _DREAMY_LITTLE_ENDIAN
        return iSrc;
      #else
        return ByteSwap64(iSrc);
      #endif
    };

    inline s16 ToLittle(s16 iSrc)
    {
      #if _DREAMY_BYTE_ORDER == _DREAMY_LITTLE_ENDIAN
        return iSrc;
      #else
        return ByteSwap16(iSrc);
      #endif
    };

    inline s32 ToLittle(s32 iSrc)
    {
      #if _DREAMY_BYTE_ORDER == _DREAMY_LITTLE_ENDIAN
        return iSrc;
      #else
        return ByteSwap32(iSrc);
      #endif
    };

    inline s64 ToLittle(s64 iSrc)
    {
      #if _DREAMY_BYTE_ORDER == _DREAMY_LITTLE_ENDIAN
        return iSrc;
      #else
        return ByteSwap64(iSrc);
      #endif
    };

    inline f32 ToLittle(f32 fSrc)
    {
      #if _DREAMY_BYTE_ORDER == _DREAMY_LITTLE_ENDIAN
        return fSrc;
      #else
        u32 *piSrc = reinterpret_cast<u32 *>(&fSrc);
        *piSrc = ByteSwap32(*piSrc);
        return fSrc;
      #endif
    };

    inline f64 ToLittle(f64 fSrc)
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

    inline size_t ToLittle(size_t iSrc)
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

    inline u16 ToBig(u16 iSrc)
    {
      #if _DREAMY_BYTE_ORDER == _DREAMY_BIG_ENDIAN
        return iSrc;
      #else
        return ByteSwap16(iSrc);
      #endif
    };

    inline u32 ToBig(u32 iSrc)
    {
      #if _DREAMY_BYTE_ORDER == _DREAMY_BIG_ENDIAN
        return iSrc;
      #else
        return ByteSwap32(iSrc);
      #endif
    };

    inline u64 ToBig(u64 iSrc)
    {
      #if _DREAMY_BYTE_ORDER == _DREAMY_BIG_ENDIAN
        return iSrc;
      #else
        return ByteSwap64(iSrc);
      #endif
    };

    inline s16 ToBig(s16 iSrc)
    {
      #if _DREAMY_BYTE_ORDER == _DREAMY_BIG_ENDIAN
        return iSrc;
      #else
        return ByteSwap16(iSrc);
      #endif
    };

    inline s32 ToBig(s32 iSrc)
    {
      #if _DREAMY_BYTE_ORDER == _DREAMY_BIG_ENDIAN
        return iSrc;
      #else
        return ByteSwap32(iSrc);
      #endif
    };

    inline s64 ToBig(s64 iSrc)
    {
      #if _DREAMY_BYTE_ORDER == _DREAMY_BIG_ENDIAN
        return iSrc;
      #else
        return ByteSwap64(iSrc);
      #endif
    };

    inline f32 ToBig(f32 fSrc)
    {
      #if _DREAMY_BYTE_ORDER == _DREAMY_BIG_ENDIAN
        return fSrc;
      #else
        u32 *piSrc = reinterpret_cast<u32 *>(&fSrc);
        *piSrc = ByteSwap32(*piSrc);
        return fSrc;
      #endif
    };

    inline f64 ToBig(f64 fSrc)
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

    inline size_t ToBig(size_t iSrc)
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
  };

};

#endif // (Dreamy Utilities Include Guard)
