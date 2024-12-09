//! This file is a part of Dreamy Utilities.
//! Licensed under the MIT license (see LICENSE file).

#ifndef _DREAMYUTILITIES_INCL_DATADUMP_H
#define _DREAMYUTILITIES_INCL_DATADUMP_H

#include "../DreamyUtilitiesBase.hpp"

#include "../Types/String.hpp"
#include "../IO/StringStream.hpp"

namespace dreamy {

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

}; // namespace dreamy

#endif // (Dreamy Utilities Include Guard)
