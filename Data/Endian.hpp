//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_ENDIAN_H
#define _DREAMYUTILITIES_INCL_ENDIAN_H

#include "../DreamyUtilitiesBase.hpp"

namespace dreamy {

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
namespace endian {

u16 ToLittle(u16 iSrc);
u32 ToLittle(u32 iSrc);
u64 ToLittle(u64 iSrc);
s16 ToLittle(s16 iSrc);
s32 ToLittle(s32 iSrc);
s64 ToLittle(s64 iSrc);
f32 ToLittle(f32 fSrc);
f64 ToLittle(f64 fSrc);

u16 ToBig(u16 iSrc);
u32 ToBig(u32 iSrc);
u64 ToBig(u64 iSrc);
s16 ToBig(s16 iSrc);
s32 ToBig(s32 iSrc);
s64 ToBig(s64 iSrc);
f32 ToBig(f32 fSrc);
f64 ToBig(f64 fSrc);

#if _DREAMY_UNIX
size_t ToLittle(size_t iSrc);
size_t ToBig(size_t iSrc);
#endif

};

};

#endif // (Dreamy Utilities Include Guard)
