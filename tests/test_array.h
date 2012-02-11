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

#include <libcds/array.h>

using namespace cds_basic;

bool cmp_array(Array &a1, Array &a2) {
  if(a1.getLength()!=a2.getLength()) return false;
  for(cds_word i=0;i<a1.getLength();i++)
    if(a1[i] != a2[i]) return false;
  return true;
}

TEST(Array,Empty) {
  Array * a = new Array(0ul,0ul);
  ASSERT_EQ(a->getLength(),0ul);
  a->unuse();
}

void testOneElem(cds_word bits) {
  cds_word v = rand() & (((cds_word)1<<bits)-1);
  Array * a = new Array(1ul,((cds_word)1ul<<bits)-1);
  a->setField(0,v);
  cds_word r = a->getField(0);
  cds_word r2 = (*a)[0];
  ASSERT_EQ(v,r);
  ASSERT_EQ(v,r2);
  a->unuse();
}

TEST(Array,OneElem) {
  for(cds_word i=0;i<msb(MAX_CDS_WORD);i++)
    testOneElem(i);
}

void testThreeElem(cds_word bits) {
  cds_word v1 = rand() & (((cds_word)1<<bits)-1);
  cds_word v2 = rand() & (((cds_word)1<<bits)-1);
  cds_word v3 = rand() & (((cds_word)1<<bits)-1);
  Array * a = new Array(3ul,bits);
  a->setField(0,v1);
  a->setField(1,v2);
  a->setField(2,v3);
  cds_word r1a = a->getField(0);
  cds_word r1b = (*a)[0];
  cds_word r2a = a->getField(1);
  cds_word r2b = (*a)[1];
  cds_word r3a = a->getField(2);
  cds_word r3b = (*a)[2];
  ASSERT_EQ(v1,r1a);
  ASSERT_EQ(v1,r1b);
  ASSERT_EQ(v2,r2a);
  ASSERT_EQ(v2,r2b);
  ASSERT_EQ(v3,r3a);
  ASSERT_EQ(v3,r3b);
  a->unuse();
}

TEST(Array,ThreeElem) {
  for(cds_word i=1;i<msb(MAX_CDS_WORD);i++) {
    testThreeElem(i);
  }
}

void testArrayConstructor1(cds_word bits) {
  cds_word v1 = rand() & (((cds_word)1<<bits)-1);
  cds_word v2 = rand() & (((cds_word)1<<bits)-1);
  cds_word v3 = rand() & (((cds_word)1<<bits)-1);
  cds_word A[3] = {v1,v2,v3};
  Array * a = new Array(A,0,3,bits);
  cds_word r1a = a->getField(0);
  cds_word r1b = (*a)[0];
  cds_word r2a = a->getField(1);
  cds_word r2b = (*a)[1];
  cds_word r3a = a->getField(2);
  cds_word r3b = (*a)[2];
  ASSERT_EQ(v1,r1a);
  ASSERT_EQ(v1,r1b);
  ASSERT_EQ(v2,r2a);
  ASSERT_EQ(v2,r2b);
  ASSERT_EQ(v3,r3a);
  ASSERT_EQ(v3,r3b);
  a->unuse();
}

void testArrayConstructor2(cds_word bits) {
  cds_word v1 = rand() & (((cds_word)1<<bits)-1);
  cds_word v2 = rand() & (((cds_word)1<<bits)-1);
  cds_word v3 = rand() & (((cds_word)1<<bits)-1);
  cds_word A[3] = {v1,v2,v3};
  Array * a = new Array(A,0,3);
  cds_word r1a = a->getField(0);
  cds_word r1b = (*a)[0];
  cds_word r2a = a->getField(1);
  cds_word r2b = (*a)[1];
  cds_word r3a = a->getField(2);
  cds_word r3b = (*a)[2];
  ASSERT_EQ(v1,r1a);
  ASSERT_EQ(v1,r1b);
  ASSERT_EQ(v2,r2a);
  ASSERT_EQ(v2,r2b);
  ASSERT_EQ(v3,r3a);
  ASSERT_EQ(v3,r3b);
  a->unuse();
}

TEST(Array,ArrayConstructor) {
  for(cds_word i=1;i<msb(MAX_CDS_WORD);i++) {
    testArrayConstructor1(i);
    testArrayConstructor2(i);
  }
}
