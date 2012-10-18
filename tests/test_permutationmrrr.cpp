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
#include <libcds/array.h>
#include <libcds/immutable/permutation.h>

using cds::immutable::PermutationMRRR;
using cds::basic::Array;
using cds::basic::ArrayTpl;
using cds::basic::cds_word;


TEST(PermutationMRRR, Direct) {
  cds_word a[] = {0,1,2,3,4,5,6,7,8,9};
  Array *perm_a = Array::Create((cds_word*)a, 0, 9);
  PermutationMRRR *perm = new PermutationMRRR(perm_a, 4);
  for (cds_word i = 0; i < perm->GetLength(); i++) {
    ASSERT_EQ(i, perm->Access(i));
    ASSERT_EQ(i, perm->Reverse(i));
  }
}

TEST(PermutationMRRR, Reverse) {
  cds_word a[] = {9,8,7,6,5,4,3,2,1,0};
  Array *perm_a = Array::Create((cds_word*)a, 0, 9);
  PermutationMRRR *perm = new PermutationMRRR(perm_a, 4);
  for (cds_word i = 0; i < perm->GetLength(); i++) {
    cds_word expected = 9-i;
    ASSERT_EQ(expected, perm->Access(i));
    ASSERT_EQ(expected, perm->Reverse(i));
  }
}

TEST(PermutationMRRR, Cycle) {
  cds_word a[] = {1,2,3,4,5,6,7,8,9,0};
  Array *perm_a = Array::Create((cds_word*)a, 0, 9);
  PermutationMRRR *perm = new PermutationMRRR(perm_a, 1);
  for (cds_word i = 0; i < perm->GetLength(); i++) {
    cds_word expected = (i+1) % 10;
    ASSERT_EQ(expected, perm->Access(i));
    expected = (i + 10 -1) % 10;
    ASSERT_EQ(expected, perm->Reverse(i));
  }
}

TEST(PermutationMRRR, Cycle2) {
  cds_word a[] = {1,2,3,4,5,6,7,8,9,0};
  Array *perm_a = Array::Create((cds_word*)a, 0, 9);
  PermutationMRRR *perm = new PermutationMRRR(perm_a, 2);
  for (cds_word i = 0; i < perm->GetLength(); i++) {
    cds_word expected = (i+1) % 10;
    ASSERT_EQ(expected, perm->Access(i));
    expected = (i + 10 -1) % 10;
    ASSERT_EQ(expected, perm->Reverse(i));
  }
}

TEST(PermutationMRRR, Cycle3) {
  cds_word a[] = {1,2,3,4,5,6,7,8,9,0};
  Array *perm_a = Array::Create((cds_word*)a, 0, 9);
  PermutationMRRR *perm = new PermutationMRRR(perm_a, 3);
  for (cds_word i = 0; i < perm->GetLength(); i++) {
    cds_word expected = (i+1) % 10;
    ASSERT_EQ(expected, perm->Access(i));
    expected = (i + 10 -1) % 10;
    ASSERT_EQ(expected, perm->Reverse(i));
  }
}

TEST(PermutationMRRR, Cycle4) {
  cds_word a[] = {1,2,3,4,5,6,7,8,9,0};
  Array *perm_a = Array::Create((cds_word*)a, 0, 9);
  PermutationMRRR *perm = new PermutationMRRR(perm_a, 4);
  for (cds_word i = 0; i < perm->GetLength(); i++) {
    cds_word expected = (i+1) % 10;
    ASSERT_EQ(expected, perm->Access(i));
    expected = (i + 10 -1) % 10;
    ASSERT_EQ(expected, perm->Reverse(i));
  }
}

TEST(PermutationMRRR, Cycle5) {
  cds_word a[] = {1,2,3,4,5,6,7,8,9,0};
  Array *perm_a = Array::Create((cds_word*)a, 0, 9);
  PermutationMRRR *perm = new PermutationMRRR(perm_a, 5);
  for (cds_word i = 0; i < perm->GetLength(); i++) {
    cds_word expected = (i+1) % 10;
    ASSERT_EQ(expected, perm->Access(i));
    expected = (i + 10 -1) % 10;
    ASSERT_EQ(expected, perm->Reverse(i));
  }
}

TEST(PermutationMRRR, Cycle6) {
  cds_word a[] = {1,2,3,4,5,6,7,8,9,0};
  Array *perm_a = Array::Create((cds_word*)a, 0, 9);
  PermutationMRRR *perm = new PermutationMRRR(perm_a, 6);
  for (cds_word i = 0; i < perm->GetLength(); i++) {
    cds_word expected = (i+1) % 10;
    ASSERT_EQ(expected, perm->Access(i));
    expected = (i + 10 -1) % 10;
    ASSERT_EQ(expected, perm->Reverse(i));
  }
}

TEST(PermutationMRRR, Cycle7) {
  cds_word a[] = {1,2,3,4,5,6,7,8,9,0};
  Array *perm_a = Array::Create((cds_word*)a, 0, 9);
  PermutationMRRR *perm = new PermutationMRRR(perm_a, 7);
  for (cds_word i = 0; i < perm->GetLength(); i++) {
    cds_word expected = (i+1) % 10;
    ASSERT_EQ(expected, perm->Access(i));
    expected = (i + 10 -1) % 10;
    ASSERT_EQ(expected, perm->Reverse(i));
  }
}

TEST(PermutationMRRR, Cycle9) {
  cds_word a[] = {1,2,3,4,5,6,7,8,9,0};
  Array *perm_a = Array::Create((cds_word*)a, 0, 9);
  PermutationMRRR *perm = new PermutationMRRR(perm_a, 9);
  for (cds_word i = 0; i < perm->GetLength(); i++) {
    cds_word expected = (i+1) % 10;
    ASSERT_EQ(expected, perm->Access(i));
    expected = (i + 10 -1) % 10;
    ASSERT_EQ(expected, perm->Reverse(i));
  }
}

TEST(PermutationMRRR, Cycle10) {
  cds_word a[] = {1,2,3,4,5,6,7,8,9,0};
  Array *perm_a = Array::Create((cds_word*)a, 0, 9);
  PermutationMRRR *perm = new PermutationMRRR(perm_a, 10);
  for (cds_word i = 0; i < perm->GetLength(); i++) {
    cds_word expected = (i+1) % 10;
    ASSERT_EQ(expected, perm->Access(i));
    expected = (i + 10 -1) % 10;
    ASSERT_EQ(expected, perm->Reverse(i));
  }
}
