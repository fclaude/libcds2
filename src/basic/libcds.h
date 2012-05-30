/********************************************************************************
Copyright (c) 2012, Rodrigo Gonzalez.
Copyright (c) 2012, Francisco Claude.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

    * Neither the name of libcds nor the names of its
      contributors may be used to endorse or promote products derived from this
      software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

********************************************************************************/


#ifndef SRC_BASIC_LIBCDS_H_
#define SRC_BASIC_LIBCDS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#include <cassert>
#include <iostream>

namespace cds {
namespace basic {

/** We define our own types for uchar, uint and ulong. */
typedef unsigned char cds_uchar;
typedef unsigned short cds_ushort;
typedef unsigned int cds_uint;
typedef size_t cds_ulong;

/** Libcds works with words represented by the type cds_word.
    Note that positions are also represented with this type.
 */
typedef size_t cds_word;

/** Number of bits in a word in libcds. */
const cds_word kWordSize = (8 * sizeof(cds_word));
const cds_word kDoubleWordSize = (16 * sizeof(cds_word));

/** Number of bytes in a word in libcds. */
const cds_word kBytesPerWord = (sizeof(cds_word));

/** Maximum values for different types. */
const cds_word kMaxCDSUchar = 255;
#if __LP64__
const cds_word kMaxCDSUint = (((cds_word)1) << (8 * sizeof(cds_uint))) - 1;
#else
const cds_word kMaxCDSUint = ((((cds_word)1) << ((8 * sizeof(cds_uint) - 1))) - 1) * 2 + 1;
#endif
const cds_word kMaxCDSUshort = 65535;
const cds_word kMaxCDSUlong = (((cds_word)1) << (8 * sizeof(cds_ulong) - 6));
const cds_word kMaxCDSWord = kMaxCDSUlong;

/** Bunch of functions to set and get individual bits from an array of words. */

/** Retrieve a given index from array A where every value uses len bits
 * @param e Array
 * @param p position
 */
inline bool BitGet(const cds_word *e, const cds_word p) {
  return 0 != ((e[p / kWordSize] >> (p % kWordSize)) & 1);
}

/** Retrieve a given index from array A where every value uses len bits
 * @param e Array
 * @param p position
 */
inline void BitOne(cds_word *e, const cds_word p) {
  e[p / kWordSize] |= ((cds_word)1 << (p % kWordSize));
}

/** Retrieve a given index from array A where every value uses len bits
 * @param e Array
 * @param p position
 */
inline void BitZero(cds_word *e, const cds_word p) {
  e[p / kWordSize] &= ~((cds_word)1 << (p % kWordSize));
}

/** Retrieve a given index from array A where every value uses len bits
 * @param e Array
 * @param p positions
 * @param bit value
 */
inline void BitSet(cds_word *e, const cds_word p, const bool bit = true) {
  if (bit) {
    BitOne(e, p);
  } else {
    BitZero(e, p);
  }
}

/** Functions to set and retrieve sequences of bits from a given array. */

/** Obtains the number of words required to represent n elements of b bits each
 */
inline cds_word WordsLength(cds_word n, cds_word b) {
  return (n * b + kWordSize - 1) / kWordSize;
}

inline cds_word SafeCeil(cds_word k, cds_word n) {
  return (k + n - 1) / n;
}

/** Retrieve a given bitsequence from array A
 * @param A Array
 * @param ini Starting position
 * @param fin Retrieve until end-1
 */
inline cds_word GetVarField(const cds_word *A, const cds_word ini, const cds_word fin) {
  if (ini == fin + 1) {
    return 0;
  }
  cds_word i = ini / kWordSize, j = ini - kWordSize * i;
  cds_word result;
  cds_word len = (fin - ini + 1);
  if (j + len <= kWordSize) {
    result = (A[i] << (kWordSize - j - len)) >> (kWordSize - len);
  } else {
    result = A[i] >> j;
    result = result | (A[i + 1] << (kDoubleWordSize - j - len)) >> (kWordSize - len);
  }
  return result;
}

/** Stores a given bitsequence into array A
 * @param A Array
 * @param ini Starting position
 * @param fin Store until end-1
 * @param x Value to be stored
 */
inline void SetVarField(cds_word *A, const cds_word ini, const cds_word fin, const cds_word x) {
  if (ini == fin + 1) {
    return;
  }
  cds_word i = ini / kWordSize, j = ini - i * kWordSize;
  cds_word len = (fin - ini + 1);
  cds_word mask = ((j + len) < kWordSize ? ~(cds_word)0 << (j + len) : 0)
                  | ((kWordSize - j) < kWordSize ? ~(cds_word)0 >> (kWordSize - j) : 0);
  A[i] = (A[i] & mask) | x << j;
  if (j + len > kWordSize) {
    mask = ((~(cds_word)0) << (len + j - kWordSize));
    A[i + 1] = (A[i + 1] & mask) | x >> (kWordSize - j);
  }
}

/** Retrieve a given index from array A where every value uses len bits
 * @param A Array
 * @param len Length in bits of each field
 * @param index Position to be retrieved
 */
inline cds_word GetField(const cds_word *A, const cds_word len, const cds_word index) {
  if (len == 0) {
    return 0;
  }
  cds_word ilen = index * len;
  cds_word i = ilen / kWordSize;
  cds_word j = ilen - kWordSize * i;
  cds_word result;
  if (j + len <= kWordSize) {
    result = (A[i] << (kWordSize - j - len)) >> (kWordSize - len);
  } else {
    result = A[i] >> j;
    result = result | (A[i + 1] << (kDoubleWordSize - j - len)) >> (kWordSize - len);
  }
  return result;
}

/** Store a given value in index into array A where every value uses len bits
 * @param A Array
 * @param len Length in bits of each field
 * @param index Position to store in
 * @param x Value to be stored
 */
inline void SetField(cds_word *A, const cds_word len, const cds_word index, const cds_word x) {
  if (len == 0) {
    return;
  }
  cds_word i = index * len / kWordSize, j = index * len - i * kWordSize;
  cds_word mask = ((j + len) < kWordSize ? ~(cds_word)0 << (j + len) : 0)
                  | ((kWordSize - j) < kWordSize ? ~(cds_word)0 >> (kWordSize - j) : 0);
  A[i] = (A[i] & mask) | x << j;
  if (j + len > kWordSize) {
    mask = ((~(cds_word)0) << (len + j - kWordSize));
    A[i + 1] = (A[i + 1] & mask) | x >> (kWordSize - j);
  }
}


/** Retrieve a given index from array A where every value uses 8 bits
 * @param A Array
 * @param index Position to be retrieved
 */
inline cds_word GetField8(const cds_word *A, const cds_word index) {
  const uint8_t *B = (const uint8_t *)A;
  return B[index];
}

/** Retrieve a given index from array A where every value uses 16 bits
 * @param A Array
 * @param index Position to be retrieved
 */
inline cds_word GetField16(const cds_word *A, const cds_word index) {
  const uint16_t *B = (const uint16_t *)A;
  return B[index];
}

/** Retrieve a given index from array A where every value uses 32 bits
 * @param A Array
 * @param index Position to be retrieved
 */
inline cds_word GetField32(const cds_word *A, const cds_word index) {
  const uint32_t *B = (const uint32_t *)A;
  return B[index];
}

/** Retrieve a given index from array A where every value uses 2 bits
 * @param A Array
 * @param index Position to be retrieved
 */
inline cds_word GetField2(const cds_word *A, const cds_word index) {
  cds_word i = index / 4, j = index % 4;
  return (GetField8(A, i) >> (2 * j)) & 3;
}

/** Retrieve a given index from array A where every value uses 4 bits
 * @param A Array
 * @param index Position to be retrieved
 */
inline cds_word GetField4(const cds_word *A, const cds_word index) {
  cds_word i = index / 2;
  if ((index & 1) == 0) {
    return GetField8(A, i) & 15;
  } else {
    return (GetField8(A, i) >> 4);
  }
}

/** We borrow some macros from the kernel :-) */
#define likely(x)       __builtin_expect((x), 1)
#define unlikely(x)     __builtin_expect((x), 0)

/** Bits required to represent a number between 0 and n */
#if __LP64__
inline cds_word msb(cds_word n) {
  cds_word ret = 0;
  if (unlikely(n == 0)) {
    return 0;
  }
  asm("bsr %1, %0" : "=r"(ret) : "r"(n));
  return ret + 1;
}
#else
inline cds_word msb(cds_word n) {
  cds_word ret = 0;
  if (unlikely(n == 0)) {
    return 0;
  }
  asm("bsr %1, %0" : "=r"(ret) : "r"(n));
  return ret + 1;
}
#endif

/** Counts the number of 1s in x
 * @param x cds_word type
 */
// #define popcount (cds_word)__builtin_popcountl
inline cds_word popcount(cds_word x) {
  if (unlikely(x == 0)) {
    return 0;
  }
  return __builtin_popcountl(x);
}

/** Obtaines the index of the least significant 1-bit of x>1 */
inline cds_word lsb(cds_word x) {
  if (unlikely(x == 0)) {
    return sizeof(cds_word) * 8;
  }
  return __builtin_ffsl(x) - 1;
}

/** Temporary implementation for select */
inline cds_word select(cds_word x, cds_word pos) {
  if (pos == 0) {
    return (cds_word) - 1;
  }
  cds_word i = 0;
  while (true) {
    if (BitGet(&x, i)) {
      pos--;
      if (pos == 0) {
        break;
      }
    }
    i++;
  }
  return i;
}

inline void PrintBinary(cds_word x) {
  for (cds_word i = 0; i < kWordSize; i++) {
    std::cout << BitGet(&x, i);
  }
  std::cout << std::endl;
}

class ReferenceCounted {
  public:
    ReferenceCounted() {
      users_count_ = 0;
    }

    virtual ~ReferenceCounted() {}

    virtual void Unuse() {
      assert(users_count_ > 0);
      users_count_--;
      if (users_count_ == 0) {
        delete this;
      }
    }

    virtual void Use() {
      users_count_++;
    }

  protected:
    cds_word users_count_;
};
};
};


#endif  // SRC_BASIC_LIBCDS_H_
