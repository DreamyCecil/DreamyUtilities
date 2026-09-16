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

// Vector type
#define NUMVEC NUMVEC_CLASS<VECTOR_TYPE, NUMVEC_OPS_FOR>

#if !defined(DO_DIMENSION_OPERATION)
  #error Please define operation for one vector dimension as DO_DIMENSION_OPERATION(VecIndex, ThisVec, Operation, Other).
#endif

// Define operations for a certain amount of dimensions
#if NUMVEC_OPS_FOR == 1
  #define DO_VECTOR_OPERATION(ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(0, ThisVec, Operation, Other)

#elif NUMVEC_OPS_FOR == 2
  #define DO_VECTOR_OPERATION(ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(0, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(1, ThisVec, Operation, Other)

#elif NUMVEC_OPS_FOR == 3
  #define DO_VECTOR_OPERATION(ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(0, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(1, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(2, ThisVec, Operation, Other)

#elif NUMVEC_OPS_FOR == 4
  #define DO_VECTOR_OPERATION(ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(0, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(1, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(2, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(3, ThisVec, Operation, Other)

#elif NUMVEC_OPS_FOR == 5
  #define DO_VECTOR_OPERATION(ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(0, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(1, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(2, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(3, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(4, ThisVec, Operation, Other)

#elif NUMVEC_OPS_FOR == 6
  #define DO_VECTOR_OPERATION(ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(0, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(1, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(2, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(3, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(4, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(5, ThisVec, Operation, Other)

#elif NUMVEC_OPS_FOR == 7
  #define DO_VECTOR_OPERATION(ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(0, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(1, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(2, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(3, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(4, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(5, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(6, ThisVec, Operation, Other)

#elif NUMVEC_OPS_FOR == 8
  #define DO_VECTOR_OPERATION(ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(0, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(1, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(2, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(3, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(4, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(5, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(6, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(7, ThisVec, Operation, Other)

#elif NUMVEC_OPS_FOR == 9
  #define DO_VECTOR_OPERATION(ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(0, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(1, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(2, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(3, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(4, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(5, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(6, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(7, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(8, ThisVec, Operation, Other)

#elif NUMVEC_OPS_FOR == 10
  #define DO_VECTOR_OPERATION(ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(0, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(1, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(2, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(3, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(4, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(5, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(6, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(7, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(8, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(9, ThisVec, Operation, Other)

#elif NUMVEC_OPS_FOR == 11
  #define DO_VECTOR_OPERATION(ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(0, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(1, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(2, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(3, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(4, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(5, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(6, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(7, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(8, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(9, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(10, ThisVec, Operation, Other)

#elif NUMVEC_OPS_FOR == 12
  #define DO_VECTOR_OPERATION(ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(0, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(1, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(2, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(3, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(4, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(5, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(6, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(7, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(8, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(9, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(10, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(11, ThisVec, Operation, Other)

#elif NUMVEC_OPS_FOR == 13
  #define DO_VECTOR_OPERATION(ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(0, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(1, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(2, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(3, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(4, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(5, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(6, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(7, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(8, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(9, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(10, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(11, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(12, ThisVec, Operation, Other)

#elif NUMVEC_OPS_FOR == 14
  #define DO_VECTOR_OPERATION(ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(0, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(1, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(2, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(3, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(4, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(5, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(6, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(7, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(8, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(9, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(10, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(11, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(12, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(13, ThisVec, Operation, Other)

#elif NUMVEC_OPS_FOR == 15
  #define DO_VECTOR_OPERATION(ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(0, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(1, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(2, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(3, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(4, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(5, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(6, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(7, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(8, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(9, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(10, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(11, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(12, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(13, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(14, ThisVec, Operation, Other)

#elif NUMVEC_OPS_FOR == 16
  #define DO_VECTOR_OPERATION(ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(0, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(1, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(2, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(3, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(4, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(5, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(6, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(7, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(8, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(9, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(10, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(11, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(12, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(13, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(14, ThisVec, Operation, Other) \
    DO_DIMENSION_OPERATION(15, ThisVec, Operation, Other)

#else
  #error Cannot have more than 16 dimensions for optimized arithmetic operators of numeric vectors.
#endif
