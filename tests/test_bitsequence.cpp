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
#include <libcds/immutable/bitsequence.h>
#include <libcds/immutable/bitsequenceseq.h>
#include <libcds/immutable/bitsequenceonelevelrank.h>
#include "./test_bitsequence_utils.h"

using cds::immutable::BitSequence;
using cds::immutable::BitSequenceOneLevelRank;
using cds::immutable::BitSequenceSeq;
using cds::basic::Array;
using cds::basic::cds_word;
using std::ofstream;

void testBitSequenceOneLevelRank(cds_word sample) {
  const cds_word kBitmapLength = 10000;
  const cds_word kOnes = kBitmapLength / 4;
  unsigned int seed = 101;
  Array *a = CreateRandomBitmap(kBitmapLength, kOnes, seed);
  a->Use();
  BitSequenceSeq *seq_bitseq = new BitSequenceSeq(a);
  seq_bitseq->Use();
  BitSequence *bs = new BitSequenceOneLevelRank(a, sample);
  bs->Use();
  TestBitSequence(seq_bitseq, bs);
  bs->Unuse();
  a->Unuse();
  seq_bitseq->Unuse();
}

TEST(BitSequence, BitSequenceOneLevelRank1) {
  testBitSequenceOneLevelRank(1);
}

TEST(BitSequence, BitSequenceOneLevelRank7) {
  testBitSequenceOneLevelRank(7);
}

TEST(BitSequence, BitSequenceOneLevelRank20) {
  testBitSequenceOneLevelRank(20);
}

TEST(BitSequence, BitSequenceOneLevelRank32) {
  testBitSequenceOneLevelRank(32);
}

TEST(BitSequence, BitSequenceOneLevelRank33) {
  testBitSequenceOneLevelRank(33);
}

TEST(BitSequence, BitSequenceOneLevelRank40) {
  testBitSequenceOneLevelRank(40);
}
