//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_BYTESWAP_H
#define _DREAMYUTILITIES_INCL_BYTESWAP_H

#include "../Base/Base.hpp"

// Byte-swapping functions for modern MSVC compilers
#if !_DREAMY_UNIX && _DREAMY_CPP11
  #include <intrin.h>
#endif

namespace dreamy {

// Invert bytes of any class' content
template<typename Type> inline
Type ByteSwapT(Type valSrc)
{
  typedef struct {
    union {
      Type _val;
      c8 _bytes[sizeof(Type)];
    };
  } SwapData;

  SwapData src;
  SwapData dst;
  src._val = valSrc;

  for (u32 i = 0; i < sizeof(Type); ++i)
  {
    dst._bytes[i] = src._bytes[sizeof(Type) - i - 1];
  }

  return dst._val;
};

// Byte-swapping functions for old MSVC compilers
#if !_DREAMY_UNIX && !_DREAMY_CPP11
  // Invert bytes of a 16-bit integer
  static inline u16 _byteswap_ushort(u16 i16) {
    return ((i16 & 0xFF00) >> 8) | ((i16 & 0x00FF) << 8);
  };

  // Invert bytes of a 32-bit integer
  static inline u32 _byteswap_ulong(u32 i32) {
    return ((i32 & 0xFF000000) >> 24)
         | ((i32 & 0x00FF0000) >>  8)
         | ((i32 & 0x0000FF00) <<  8)
         | ((i32 & 0x000000FF) << 24);
  };

  // Invert bytes of a 64-bit integer
  #define _byteswap_uint64(i64) ByteSwapT<u64>(i64)
#endif

// Reverse the order of bytes in a 16-bit integer
__forceinline u16 ByteSwap16(u16 i16)
{
  #if !_DREAMY_UNIX
    return _byteswap_ushort(i16);
  #else
    return __builtin_bswap16(i16);
  #endif
};

// Reverse the order of bytes in a 32-bit integer
__forceinline u32 ByteSwap32(u32 i32)
{
  #if !_DREAMY_UNIX
    return _byteswap_ulong(i32);
  #else
    return __builtin_bswap32(i32);
  #endif
};

// Reverse the order of bytes in a 64-bit integer
__forceinline u64 ByteSwap64(u64 i64)
{
  #if !_DREAMY_UNIX
    return _byteswap_uint64(i64);
  #else
    return __builtin_bswap64(i64);
  #endif
};

};

#endif // (Dreamy Utilities Include Guard)
