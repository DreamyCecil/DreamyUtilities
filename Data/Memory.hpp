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

#ifndef _DREAMYUTILITIES_INCL_MEMORY_H
#define _DREAMYUTILITIES_INCL_MEMORY_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

#include "../Types/String.hpp"

#include <cstdlib>

// Byte-swapping functions for modern MSVC compilers
#if !_DREAMY_UNIX && _DREAMY_CPP11
  #include <intrin.h>
#endif

NAMESPACE_DREAMY_OPEN

// Invert bytes of any structure
template<typename Type>
inline Type ByteSwapT(const Type &valSrc)
{
  typedef struct {
    union {
      Type val;
      c8 bytes[sizeof(Type)];
    };
  } SwapData;

  SwapData src;
  SwapData dst;
  src.val = valSrc;

  for (u32 i = 0; i < sizeof(Type); ++i)
  {
    dst.bytes[i] = src.bytes[sizeof(Type) - i - 1];
  }

  return dst.val;
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

// Resize array by constructing values in place and moving the data bytes
template<typename Type>
inline void ResizeBuffer_memcpy(Type **pBuffer, size_t iOldSize, size_t iNewSize) {
  Type *pNew = new Type[iNewSize];
  memcpy(pNew, *pBuffer, iOldSize * sizeof(Type));

  delete[] *pBuffer;
  *pBuffer = pNew;
};

// Resize array by constructing and reassigning values in place
template<typename Type>
inline void ResizeBuffer_new(Type **pBuffer, size_t iOldSize, size_t iNewSize) {
  Type *pNew = new Type[iNewSize];

  // Reassign old values
  while (--iOldSize >= 0) {
    pNew[iOldSize] = (*pBuffer)[iOldSize];
  }

  delete[] *pBuffer;
  *pBuffer = pNew;
};

// Resize array by reallocating C dynamic memory
template<typename Type>
inline void ResizeBuffer_realloc(Type **pBuffer, size_t iNewSize) {
  *pBuffer = (Type *)realloc(*pBuffer, iNewSize * sizeof(Type));
};

NAMESPACE_DREAMY_CLOSE

#endif // (Dreamy Utilities Include Guard)
