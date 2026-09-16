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

#ifndef _DREAMYUTILITIES_INCL_DATADUMP_H
#define _DREAMYUTILITIES_INCL_DATADUMP_H
#ifdef _WIN32
  #pragma once
#endif

#include "../DreamyUtilitiesBase.hpp"

#include "../Types/String.hpp"
#include "../IO/StringStream.hpp"

NAMESPACE_DREAMY_OPEN

// Display raw data divided into multiple byte chunks
void DumpDataChunks(CStringStream &out, void *pData, size_t iStartOffset, size_t iChunkSize, size_t iChunks);

// Print unsigned integers
CString DumpField(const u8 *pField, const c8 *strPrintAs = "%u");
CString DumpField(const u16 *pField, const c8 *strPrintAs = "%u");
CString DumpField(const u32 *pField, const c8 *strPrintAs = "%u");
CString DumpField(const u64 *pField, const c8 *strPrintAs = "%llu");

// Print signed integers
CString DumpField(const s8 *pField, const c8 *strPrintAs = "%d");
CString DumpField(const s16 *pField, const c8 *strPrintAs = "%d");
CString DumpField(const s32 *pField, const c8 *strPrintAs = "%d");
CString DumpField(const s64 *pField, const c8 *strPrintAs = "%lld");

// Print string
CString DumpField(const c8 **pField, const c8 *strPrintAs = "%s");

NAMESPACE_DREAMY_CLOSE

#endif // (Dreamy Utilities Include Guard)
