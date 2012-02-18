/********************************************************************************
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

#include <gtest/gtest.h>
#include <libcds/libcds.h>

#include <algorithm>

using std::sort;
using cds::basic::msb;
using cds::basic::lsb;
using cds::basic::cds_word;
using cds::basic::cds_uint;
using cds::basic::cds_uchar;
using cds::basic::kMaxCDSUchar;
using cds::basic::kMaxCDSUshort;
using cds::basic::kMaxCDSUint;
using cds::basic::kMaxCDSWord;
using cds::basic::kMaxCDSUlong;
using cds::basic::BitZero;
using cds::basic::BitGet;
using cds::basic::BitSet;
using cds::basic::kWordSize;
using cds::basic::kDoubleWordSize;
using cds::basic::SetField;
using cds::basic::SetVarField;
using cds::basic::GetField;
using cds::basic::GetVarField;
using cds::basic::GetField8;
using cds::basic::GetField16;
using cds::basic::GetField32;

uint seed = 2;

/** Test values are what we expect */
TEST(TypeSize, ExpectedValues) {
  unsigned long long v;
  v = (((unsigned long long)1) << 8) - 1;
  ASSERT_EQ(v, kMaxCDSUchar);
  v = (((unsigned long long)1) << (8 * 2)) - 1;
  ASSERT_EQ(v, kMaxCDSUshort);
  v = (((unsigned long long)1) << (8 * 4)) - 1;
  ASSERT_EQ(v, kMaxCDSUint);
#if __LP64__
  v = (((unsigned long long)1) << (8 * 8 - 6));
#else
  v = (((unsigned long long)1) << (8 * 4 - 6));
#endif
  ASSERT_EQ(v, kMaxCDSWord);
  ASSERT_EQ(v, kMaxCDSUlong);
#if __LP64__
  v = 64;
#else
  v = 32;
#endif
  ASSERT_EQ(v, kWordSize);
  v *= 2;
  ASSERT_EQ(v, kDoubleWordSize);
}


/** Test popcount */
cds_word seq_popcount(const cds_word x) {
  cds_word v = x;
  cds_word ret = 0;
  while (v != 0) {
    if (v & 1) {
      ret++;
    }
    v >>= 1;
  }
  return ret;
}


TEST(Bits, PopcountWord) {
  for (cds_word i = 0; i < 128 * kMaxCDSUchar; i++) {
    ASSERT_EQ(seq_popcount(i), popcount(i));
  }
  for (cds_word i = 0; i < 128 * kMaxCDSUchar; i++) {
    ASSERT_EQ(seq_popcount((i << 30) | i), popcount((i << 30) | i));
  }
}


TEST(Bits, PopcountUint) {
  for (cds_word i = 0; i < 128 * kMaxCDSUchar; i++) {
    ASSERT_EQ(seq_popcount(i), popcount((cds_uint)i));
  }
}


/** Test bitset, bitget and bitzero */
TEST(Bits, BitOperations) {
  cds_word len = 10000;
  cds_word *bm = new cds_word[len / kWordSize + 1];
  cds_word *pos = new cds_word[len / 2];
  bool t = true, f = false;
  for (cds_word i = 0; i < len / 2; i++) {
    pos[i] = ((cds_word)rand_r(&seed)) % len;
    BitZero(bm, 2 * i);
    ASSERT_EQ(f, BitGet(bm, 2 * i));
    BitSet(bm, 2 * i + 1, true);
    ASSERT_EQ(t, BitGet(bm, 2 * i + 1));
    BitSet(bm, 2 * i + 1, false);
    ASSERT_EQ(f, BitGet(bm, 2 * i + 1));
  }
  sort(pos, pos + len / 2);
  for (cds_word i = 0; i < len / 2; i++) {
    BitSet(bm, pos[i], true);
    ASSERT_EQ(t, BitGet(bm, pos[i]));
  }
  cds_word posn = 0;
  for (cds_word i = 0; i < len / 2; i++) {
    while (posn < pos[i]) {
      ASSERT_EQ(f, BitGet(bm, posn));
      posn++;
    }
    if (posn == pos[i]) {
      ASSERT_EQ(t, BitGet(bm, posn));
      BitSet(bm, posn, false);
    }
    posn = pos[i] + 1;
  }
  for (cds_word i = 0; i < len; i++) {
    ASSERT_EQ(f, BitGet(bm, i));
  }
  delete [] bm;
  delete [] pos;
}


/** Test bits function */

/** Test msb() */
cds_word my_msb(cds_word x) {
  cds_word ret = 0;
  while (x) {
    x >>= 1;
    ret++;
  }
  return ret;
}


TEST(Bits, Msb) {
  for (cds_word i = 0; i < kMaxCDSUint / 1000; i++) {
    cds_word val = rand_r(&seed) % kMaxCDSWord;
    ASSERT_EQ(my_msb(val), msb(val));
  }
}


/** Test lsb() */
cds_word my_lsb(cds_word x) {
  cds_word ret = 0;
  if (x == 0) {
    return sizeof(cds_word) * 8;
  }
  while ((x & 1) == 0) {
    x >>= 1;
    ret++;
  }
  return ret;
}


TEST(Bits, Lsb) {
  for (cds_word c = 1; c < kMaxCDSUint / 1000; c++) {
    cds_word val = rand_r(&seed) % kMaxCDSWord;
    ASSERT_EQ(my_lsb(val), lsb(val));
  }
}


/** Test get and set fields */

TEST(Fields, ZeroBits) {
  const cds_word N = 10;
  cds_word exp = 0;
  cds_word vals[N];
  cds_word exp2 = (cds_word)0xFFFFFFFFFFFFFFFFll;
  cds_word exp3 = 1;
  for (cds_word i = 0; i < N; i++) {
    vals[i] = exp2;
  }

  for (cds_word i = 0; i < kMaxCDSUchar; i++) {
    SetField(vals, 0, i, rand_r(&seed));
    for (cds_word j = 0; j < N; j++) {
      ASSERT_EQ(exp2, vals[j]);
    }

    for (cds_word j = 0; j < N; j++) {
      ASSERT_EQ(exp2, vals[j]);
    }

    ASSERT_EQ(exp, GetField(vals, 0, i));
    ASSERT_EQ(exp3, GetVarField(vals, i, i));
  }
  for (cds_word i = 0; i < N; i++) {
    ASSERT_EQ(exp2, vals[i]);
  }
}


void test_fields_k(const cds_word k) {
  const cds_word N = 1000000;
  const cds_word array_length = N * k / kWordSize + 1;
  cds_word mask = ((cds_word)1 << k) - 1;
  cds_word A[array_length];
  for (cds_word i = 0; i < array_length; i++) {
    A[i] = 0;
  }

  for (cds_word i = 0; i < N; i++) {
    SetField(A, k, i, i & mask);
    cds_word exp = i & mask;
    ASSERT_EQ(exp, GetField(A, k, i));
  }

  for (cds_word i = 0; i < N; i++) {
    SetVarField(A, k * i, k * (i + 1) - 1, i & mask);
    cds_word exp = i & mask;
    ASSERT_EQ(exp, GetVarField(A, k * i, k * (i + 1) - 1));
  }
}


TEST(Fields, AllBits) {
  for (uint i = 1; i <= 58; i++) {
    test_fields_k(i);
  }
}


TEST(Fields, 4Bits) {
  const cds_word k = 4;
  const cds_word N = 1000000;
  const cds_word array_length = N * k / kWordSize + 1;
  cds_word mask = ((cds_word)1 << k) - 1;
  cds_word A[array_length];
  for (cds_word i = 0; i < array_length; i++) {
    A[i] = 0;
  }

  for (cds_word i = 0; i < N; i++) {
    cds_word exp = i & mask;
    SetField(A, k, i, exp);
    ASSERT_EQ(exp, GetField(A, k, i));
  }

  for (cds_word i = 0; i < N; i++) {
    cds_word exp = i & mask;
    SetVarField(A, k * i, k * (i + 1) - 1, exp);
    ASSERT_EQ(exp, GetVarField(A, k * i, k * (i + 1) - 1));
  }
}


TEST(Fields, 8Bits) {
  const cds_word k = 8;
  const cds_word N = 1000000;
  const cds_word array_length = N * k / kWordSize + 1;
  cds_word mask = ((cds_word)1 << k) - 1;
  cds_word A[array_length];
  for (cds_word i = 0; i < array_length; i++) {
    A[i] = 0;
  }

  for (cds_word i = 0; i < N; i++) {
    SetField(A, k, i, i & mask);
    cds_word exp = i & mask;
    ASSERT_EQ(exp, GetField8(A, i));
  }

  for (cds_word i = 0; i < N; i++) {
    SetVarField(A, k * i, k * (i + 1) - 1, i & mask);
    cds_word exp = i & mask;
    ASSERT_EQ(exp, GetVarField(A, k * i, k * (i + 1) - 1));
  }
}


TEST(Fields, 16Bits) {
  const cds_word k = 16;
  const cds_word N = 1000000;
  const cds_word array_length = N * k / kWordSize + 1;
  cds_word mask = ((cds_word)1 << k) - 1;
  cds_word A[array_length];
  for (cds_word i = 0; i < array_length; i++) {
    A[i] = 0;
  }

  for (cds_word i = 0; i < N; i++) {
    SetField(A, k, i, i & mask);
    cds_word exp = i & mask;
    ASSERT_EQ(exp, GetField16(A, i));
  }

  for (cds_word i = 0; i < N; i++) {
    SetVarField(A, k * i, k * (i + 1) - 1, i & mask);
    cds_word exp = i & mask;
    ASSERT_EQ(exp, GetVarField(A, k * i, k * (i + 1) - 1));
  }
}


TEST(Fields, 32Bits) {
  const cds_word k = 32;
  const cds_word N = 1000000;
  const cds_word array_length = N * k / kWordSize + 1;
#if __LP64__
  cds_word mask = ((cds_word)1 << k) - 1;
#else
  cds_word mask = kMaxCDSUint;
#endif
  cds_word A[array_length];
  for (cds_word i = 0; i < array_length; i++) {
    A[i] = 0;
  }

  for (cds_word i = 0; i < N; i++) {
    SetField(A, k, i, i & mask);
    cds_word exp = i & mask;
    ASSERT_EQ(exp, GetField32(A, i));
  }

  for (cds_word i = 0; i < N; i++) {
    SetVarField(A, k * i, k * (i + 1) - 1, i & mask);
    cds_word exp = i & mask;
    ASSERT_EQ(exp, GetVarField(A, k * i, k * (i + 1) - 1));
  }
}
