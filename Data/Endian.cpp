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

#include "Endian.hpp"
#include "Memory.hpp"

NAMESPACE_DREAMY_OPEN

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

NAMESPACE_DREAMY_CLOSE
