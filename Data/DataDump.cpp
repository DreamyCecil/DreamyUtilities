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

#include "DataDump.hpp"

#include <iomanip>

NAMESPACE_DREAMY_OPEN

void DumpDataChunks(CStringStream &out, void *pData, size_t iStartOffset, size_t iChunkSize, size_t iChunks) {
  // Pure byte array starting at some offset
  u8 *pDataBytes = ((u8 *)pData) + iStartOffset;

  // Chunk bytes
  u8 *aChunk = new u8[iChunkSize];

  // Go until the last byte
  const size_t iBytes = iStartOffset + iChunks * iChunkSize;

  for (size_t iOffset = iStartOffset; iOffset < iBytes; iOffset += iChunkSize) {
    // Display current offset as an address
    out.PrintF("%0*X", sizeof(size_t) * 2, iOffset);

    // Reset formatting and separate output space
    out << '\t';

    // Output one chunk of data at the current offset byte by byte
    for (size_t iChunk = 0; iChunk < iChunkSize; ++iChunk) {
      aChunk[iChunk] = *(pDataBytes + iOffset + iChunk);

      // Replace invalid characters with spaces
      c8 chPrint = (aChunk[iChunk] < ' ') ? ' ' : aChunk[iChunk];
      out << chPrint;
    }

    out << '\t';

    // Output one chunk of data in pure bytes
    for (size_t iPure = 0; iPure < iChunkSize; ++iPure) {
      u32 iByte = (u32)aChunk[iPure];
      out.PrintF("%02X", iByte);
    }

    // Next chunk
    out << '\n';
  }

  delete[] aChunk;
};

CString DumpField(const u8 *pField, const c8 *strPrintAs) {
  c8 str[128];
  sprintf(str, strPrintAs, *pField);
  return str;
};

CString DumpField(const u16 *pField, const c8 *strPrintAs) {
  c8 str[128];
  sprintf(str, strPrintAs, *pField);
  return str;
};

CString DumpField(const u32 *pField, const c8 *strPrintAs) {
  c8 str[128];
  sprintf(str, strPrintAs, *pField);
  return str;
};

CString DumpField(const u64 *pField, const c8 *strPrintAs) {
  c8 str[128];
  sprintf(str, strPrintAs, *pField);
  return str;
};

CString DumpField(const s8 *pField, const c8 *strPrintAs) {
  c8 str[128];
  sprintf(str, strPrintAs, *pField);
  return str;
};

CString DumpField(const s16 *pField, const c8 *strPrintAs) {
  c8 str[128];
  sprintf(str, strPrintAs, *pField);
  return str;
};

CString DumpField(const s32 *pField, const c8 *strPrintAs) {
  c8 str[128];
  sprintf(str, strPrintAs, *pField);
  return str;
};

CString DumpField(const s64 *pField, const c8 *strPrintAs) {
  c8 str[128];
  sprintf(str, strPrintAs, *pField);
  return str;
};

CString DumpField(const c8 **pField, const c8 *strPrintAs) {
  c8 str[128];
  sprintf(str, strPrintAs, *pField);
  return str;
};

NAMESPACE_DREAMY_CLOSE
