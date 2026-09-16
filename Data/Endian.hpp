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

#ifndef _DREAMYUTILITIES_INCL_ENDIAN_H
#define _DREAMYUTILITIES_INCL_ENDIAN_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

NAMESPACE_DREAMY_OPEN

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

}; // namespace endian

NAMESPACE_DREAMY_CLOSE

#endif // (Dreamy Utilities Include Guard)
