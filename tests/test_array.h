/**
 *    test_array.h
 *    Copyright (C) 2011  Francisco Claude F.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TESTS_TEST_ARRAY_H_
#define TESTS_TEST_ARRAY_H_

#include <libcds/array.h>

using cds::basic::Array;

uint seed_test_array = 20;

bool cmp_array(Array &a1, Array &a2) {
  if (a1.GetLength() != a2.GetLength()) {
    return false;
  }
  for (cds_word i = 0; i < a1.GetLength(); i++)
    if (a1[i] != a2[i]) {
      return false;
    }
  return true;
}


TEST(Array, Empty) {
  Array *a = new Array(0ul, 0ul);
  ASSERT_EQ(a->GetLength(), 0ul);
  a->Unuse();
}


void testOneElem(cds_word bits) {
  cds_word v = rand_r(&seed_test_array) & (((cds_word)1 << bits) - 1);
  Array *a = new Array(1ul, ((cds_word)1ul << bits) - 1);
  a->SetField(0, v);
  cds_word r = a->GetField(0);
  cds_word r2 = (*a)[0];
  ASSERT_EQ(v, r);
  ASSERT_EQ(v, r2);
  a->Unuse();
}


TEST(Array, OneElem) {
  for (cds_word i = 0; i < msb(kMaxCDSWord); i++) {
    testOneElem(i);
  }
}


void testThreeElem(cds_word bits) {
  cds_word v1 = rand_r(&seed_test_array) & (((cds_word)1 << bits) - 1);
  cds_word v2 = rand_r(&seed_test_array) & (((cds_word)1 << bits) - 1);
  cds_word v3 = rand_r(&seed_test_array) & (((cds_word)1 << bits) - 1);
  Array *a = new Array(3ul, bits);
  a->SetField(0, v1);
  a->SetField(1, v2);
  a->SetField(2, v3);
  cds_word r1a = a->GetField(0);
  cds_word r1b = (*a)[0];
  cds_word r2a = a->GetField(1);
  cds_word r2b = (*a)[1];
  cds_word r3a = a->GetField(2);
  cds_word r3b = (*a)[2];
  ASSERT_EQ(v1, r1a);
  ASSERT_EQ(v1, r1b);
  ASSERT_EQ(v2, r2a);
  ASSERT_EQ(v2, r2b);
  ASSERT_EQ(v3, r3a);
  ASSERT_EQ(v3, r3b);
  a->Unuse();
}


TEST(Array, ThreeElem) {
  for (cds_word i = 1; i < msb(kMaxCDSWord); i++) {
    testThreeElem(i);
  }
}


void testArrayConstructor1(cds_word bits) {
  cds_word v1 = rand_r(&seed_test_array) & (((cds_word)1 << bits) - 1);
  cds_word v2 = rand_r(&seed_test_array) & (((cds_word)1 << bits) - 1);
  cds_word v3 = rand_r(&seed_test_array) & (((cds_word)1 << bits) - 1);
  cds_word A[3] = {v1, v2, v3};
  Array *a = new Array(A, 0, 3, bits);
  cds_word r1a = a->GetField(0);
  cds_word r1b = (*a)[0];
  cds_word r2a = a->GetField(1);
  cds_word r2b = (*a)[1];
  cds_word r3a = a->GetField(2);
  cds_word r3b = (*a)[2];
  ASSERT_EQ(v1, r1a);
  ASSERT_EQ(v1, r1b);
  ASSERT_EQ(v2, r2a);
  ASSERT_EQ(v2, r2b);
  ASSERT_EQ(v3, r3a);
  ASSERT_EQ(v3, r3b);
  a->Unuse();
}


void testArrayConstructor2(cds_word bits) {
  cds_word v1 = rand_r(&seed_test_array) & (((cds_word)1 << bits) - 1);
  cds_word v2 = rand_r(&seed_test_array) & (((cds_word)1 << bits) - 1);
  cds_word v3 = rand_r(&seed_test_array) & (((cds_word)1 << bits) - 1);
  cds_word A[3] = {v1, v2, v3};
  Array *a = new Array(A, 0, 3);
  cds_word r1a = a->GetField(0);
  cds_word r1b = (*a)[0];
  cds_word r2a = a->GetField(1);
  cds_word r2b = (*a)[1];
  cds_word r3a = a->GetField(2);
  cds_word r3b = (*a)[2];
  ASSERT_EQ(v1, r1a);
  ASSERT_EQ(v1, r1b);
  ASSERT_EQ(v2, r2a);
  ASSERT_EQ(v2, r2b);
  ASSERT_EQ(v3, r3a);
  ASSERT_EQ(v3, r3b);
  a->Unuse();
}


TEST(Array, ArrayConstructor) {
  for (cds_word i = 1; i < msb(kMaxCDSWord); i++) {
    testArrayConstructor1(i);
    testArrayConstructor2(i);
  }
}

#endif  // TESTS_TEST_ARRAY_H_
