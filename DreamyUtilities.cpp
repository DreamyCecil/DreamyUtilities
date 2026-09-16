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

// Compile all source files in a single place for convenience
#include "Data/DataDump.cpp"
#include "Data/Endian.cpp"

#include "Hashing/CRC32.cpp"
#include "Hashing/SimpleHasher.cpp"

#include "IO/BufferDevice.cpp"
#include "IO/Console.cpp"
#include "IO/DataStream.cpp"
#include "IO/FileDevice.cpp"
#include "IO/Files.cpp"
#include "IO/ReadWriteDevice.cpp"
#include "IO/StringStream.cpp"

#include "Parser/CLikeParser.cpp"
#include "Parser/GenericParser.cpp"
#include "Parser/JSON.cpp"

#include "Types/ByteArray.cpp"
#include "Types/String.cpp"
#include "Types/Variant.cpp"
