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
#include <libcds/immutable/treelouds.h>
#include <libcds/immutable/bitsequence.h>
#include <libcds/immutable/bitsequenceonelevelrank.h>

using cds::immutable::TreeLouds;
using cds::immutable::BitSequence;
using cds::immutable::BitSequenceOneLevelRank;
using cds::basic::Array;
using cds::basic::ArrayTpl;
using cds::basic::cds_word;

#include <vector>

using std::vector;

BitSequence *createTree() {
  vector<cds_word> v(21);
  v[0] = v[1] = v[2] = v[4] = v[6] = v[7] = 1;
  v[9] = v[10] = v[14] = v[15] = 1;
  Array *a = Array::Create(v);
  // std::cout << "Tree: ";
  // for (cds_word i = 0; i < a->GetLength(); i++)
  //   std::cout << a->GetField(i);
  // std::cout << std::endl;
  return new BitSequenceOneLevelRank(a, 20);
}

TEST(TreeLouds, Child) {
  BitSequence *b = createTree();
  TreeLouds *t = new TreeLouds(b);
  t->Use();
  cds_word got, expected;

  got = t->Child(3,0);
  expected = 5;
  ASSERT_EQ(expected, got);

  got = t->Child(3,1);
  expected = 8;
  ASSERT_EQ(expected, got);

  got = t->Child(3,2);
  expected = 11;
  ASSERT_EQ(expected, got);

  got = t->Child(5,0);
  expected = 12;
  ASSERT_EQ(expected, got);

  got = t->Child(8,0);
  expected = 13;
  ASSERT_EQ(expected, got);

  got = t->Child(8,1);
  expected = 16;
  ASSERT_EQ(expected, got);

  got = t->Child(11,0);
  expected = 17;
  ASSERT_EQ(expected, got);

  got = t->Child(11,1);
  expected = 18;
  ASSERT_EQ(expected, got);

  got = t->Child(16,0);
  expected = 19;
  ASSERT_EQ(expected, got);

  got = t->Child(16,1);
  expected = 20;
  ASSERT_EQ(expected, got);

  t->Unuse();
}

TEST(TreeLouds, Parent) {
  BitSequence *b = createTree();
  TreeLouds *t = new TreeLouds(b);
  t->Use();
  cds_word got, expected;

  got = t->Parent(5);
  expected = 3;
  ASSERT_EQ(expected, got);

  got = t->Parent(8);
  expected = 3;
  ASSERT_EQ(expected, got);

  got = t->Parent(11);
  expected = 3;
  ASSERT_EQ(expected, got);

  got = t->Parent(12);
  expected = 5;
  ASSERT_EQ(expected, got);

  got = t->Parent(13);
  expected = 8;
  ASSERT_EQ(expected, got);

  got = t->Parent(16);
  expected = 8;
  ASSERT_EQ(expected, got);

  got = t->Parent(17);
  expected = 11;
  ASSERT_EQ(expected, got);

  got = t->Parent(18);
  expected = 11;
  ASSERT_EQ(expected, got);

  got = t->Parent(19);
  expected = 16;
  ASSERT_EQ(expected, got);

  got = t->Parent(20);
  expected = 16;
  ASSERT_EQ(expected, got);

  t->Unuse();
}

TEST(TreeLouds, PrevSibling) {
  BitSequence *b = createTree();
  TreeLouds *t = new TreeLouds(b);
  t->Use();
  cds_word got, expected;

  got = t->PrevSibling(5);
  expected = (cds_word)-1;
  ASSERT_EQ(expected, got);

  got = t->PrevSibling(8);
  expected = (cds_word)5;
  ASSERT_EQ(expected, got);

  got = t->PrevSibling(11);
  expected = (cds_word)8;
  ASSERT_EQ(expected, got);

  got = t->PrevSibling(13);
  expected = (cds_word)-1;
  ASSERT_EQ(expected, got);

  got = t->PrevSibling(16);
  expected = (cds_word)13;
  ASSERT_EQ(expected, got);

  got = t->PrevSibling(17);
  expected = (cds_word)-1;
  ASSERT_EQ(expected, got);

  got = t->PrevSibling(18);
  expected = (cds_word)17;
  ASSERT_EQ(expected, got);

  got = t->PrevSibling(19);
  expected = (cds_word)-1;
  ASSERT_EQ(expected, got);

  got = t->PrevSibling(20);
  expected = (cds_word)19;
  ASSERT_EQ(expected, got);

  t->Unuse();
}

TEST(TreeLouds, NextSibling) {
  BitSequence *b = createTree();
  TreeLouds *t = new TreeLouds(b);
  t->Use();
  cds_word got, expected;

  got = t->NextSibling(5);
  expected = (cds_word)8;
  ASSERT_EQ(expected, got);

  got = t->NextSibling(8);
  expected = (cds_word)11;
  ASSERT_EQ(expected, got);

  got = t->NextSibling(11);
  expected = (cds_word)-1;
  ASSERT_EQ(expected, got);

  got = t->NextSibling(13);
  expected = (cds_word)16;
  ASSERT_EQ(expected, got);

  got = t->NextSibling(16);
  expected = (cds_word)-1;
  ASSERT_EQ(expected, got);

  got = t->NextSibling(17);
  expected = (cds_word)18;
  ASSERT_EQ(expected, got);

  got = t->NextSibling(18);
  expected = (cds_word)-1;
  ASSERT_EQ(expected, got);

  got = t->NextSibling(19);
  expected = (cds_word)20;
  ASSERT_EQ(expected, got);

  got = t->NextSibling(20);
  expected = (cds_word)-1;
  ASSERT_EQ(expected, got);

  t->Unuse();
}

TEST(TreeLouds, Degree) {
  BitSequence *b = createTree();
  TreeLouds *t = new TreeLouds(b);
  t->Use();
  cds_word got, expected;

  got = t->Degree(3);
  expected = 3;
  ASSERT_EQ(expected, got);

  got = t->Degree(5);
  expected = 1;
  ASSERT_EQ(expected, got);

  got = t->Degree(8);
  expected = 2;
  ASSERT_EQ(expected, got);

  got = t->Degree(11);
  expected = 2;
  ASSERT_EQ(expected, got);

  got = t->Degree(12);
  expected = 0;
  ASSERT_EQ(expected, got);

  got = t->Degree(13);
  expected = 0;
  ASSERT_EQ(expected, got);

  got = t->Degree(16);
  expected = 2;
  ASSERT_EQ(expected, got);

  got = t->Degree(17);
  expected = 0;
  ASSERT_EQ(expected, got);

  got = t->Degree(18);
  expected = 0;
  ASSERT_EQ(expected, got);

  got = t->Degree(19);
  expected = 0;
  ASSERT_EQ(expected, got);

  got = t->Degree(20);
  expected = 0;
  ASSERT_EQ(expected, got);

  t->Unuse();
}

TEST(TreeLouds, Nodes) {
  BitSequence *b = createTree();
  TreeLouds *t = new TreeLouds(b);
  t->Use();
  cds_word got, expected;

  got = t->GetNodes();
  expected = 11;
  ASSERT_EQ(expected, got);

  t->Unuse();
}