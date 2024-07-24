//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_DATA_DUMP_H
#define _DREAMYUTILITIES_INCL_DATA_DUMP_H

#include "../Base/Base.hpp"

#include "../Strings/Strings.hpp"
#include <iomanip>

namespace dreamy {

// Display raw data divided into multiple byte chunks
inline void DumpDataChunks(CStringStream &out, void *pData, size_t iStartOffset, size_t iChunkSize, size_t iChunks) {
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

// Print unsigned integers

inline CString DumpField(const u8 *pField, const c8 *strPrintAs = "%u") {
  c8 str[128];
  sprintf(str, strPrintAs, *pField);
  return str;
};

inline CString DumpField(const u16 *pField, const c8 *strPrintAs = "%u") {
  c8 str[128];
  sprintf(str, strPrintAs, *pField);
  return str;
};

inline CString DumpField(const u32 *pField, const c8 *strPrintAs = "%u") {
  c8 str[128];
  sprintf(str, strPrintAs, *pField);
  return str;
};

inline CString DumpField(const u64 *pField, const c8 *strPrintAs = "%llu") {
  c8 str[128];
  sprintf(str, strPrintAs, *pField);
  return str;
};

// Print signed integers

inline CString DumpField(const s8 *pField, const c8 *strPrintAs = "%d") {
  c8 str[128];
  sprintf(str, strPrintAs, *pField);
  return str;
};

inline CString DumpField(const s16 *pField, const c8 *strPrintAs = "%d") {
  c8 str[128];
  sprintf(str, strPrintAs, *pField);
  return str;
};

inline CString DumpField(const s32 *pField, const c8 *strPrintAs = "%d") {
  c8 str[128];
  sprintf(str, strPrintAs, *pField);
  return str;
};

inline CString DumpField(const s64 *pField, const c8 *strPrintAs = "%lld") {
  c8 str[128];
  sprintf(str, strPrintAs, *pField);
  return str;
};

// Print string
inline CString DumpField(const c8 **pField, const c8 *strPrintAs = "%s") {
  c8 str[128];
  sprintf(str, strPrintAs, *pField);
  return str;
};

};

#endif // (Dreamy Utilities Include Guard)
