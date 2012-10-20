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
#include <libcds/immutable/sequence.h>

#include "./test_sequence_utils.h"

using cds::immutable::Sequence;
using cds::immutable::WaveletTreeNoPtrs;
using cds::immutable::MapperNone;
using cds::immutable::BitSequenceBuilderOneLevelRank;
using cds::basic::Array;
using cds::basic::ArrayTpl;
using cds::basic::cds_word;
using std::ostream;

const cds_word kLength = 5000;
const cds_word sigma[] = {1, 7759};
const cds_word sigmas = 2;
uint seed_test_sequence = 20;

class SimpleSeq : public Sequence {
  public:
    explicit SimpleSeq(Array *array) {
      array_ = array;
      array_->Use();
    }
    ~SimpleSeq() {
      array_->Unuse();
    }
    cds_word GetLength() const {
      return array_->GetLength();
    }
    cds_word GetSigma() const {
      return array_->GetMax();
    }
    cds_word Access(const cds_word i) const {
      return array_->GetField(i);
    }
    cds_word Rank(const cds_word s, const cds_word i) const {
      cds_word count = 0;
      for (cds_word pos = 0; pos <= i; pos++)
        if (array_->GetField(pos) == s) {
          count++;
        }
      return count;
    }
    cds_word Select(const cds_word s, const cds_word j) const {
      cds_word count = 0;
      for (cds_word pos = 0; pos < array_->GetLength(); pos++) {
        if (array_->GetField(pos) == s) {
          count++;
        }
        if (count == j) {
          return pos;
        }
      }
      return array_->GetLength();
    }
    cds_word Access(const cds_word i, cds_word *rank) {
      cds_word s = array_->GetField(i);
      *rank = Rank(s, i);
      return s;
    }
    cds_word GetSize() const {
      return static_cast<cds_word>(-1);
    }
    void Save(ostream &fp) const {
      assert(false);
    }

  protected:
    Array *array_;
};

class SimpleSeqAccess : public Sequence {
  public:
    explicit SimpleSeqAccess(Array *array) {
      array_ = array;
    }

    cds_word GetLength() const {
      return array_->GetLength();
    }

    cds_word Access(const cds_word i) const {
      return array_->GetField(i);
    }
    cds_word GetSize() const {
      return static_cast<cds_word>(-1);
    }
    void Save(ostream &fp) const {
      assert(false);
    }

  protected:
    Array *array_;
};

TEST(Sequence, SimpleSeqAccess) {
  for (cds_word ind = 0; ind < sigmas; ind++) {
    Array *array = CreateRandomSequence(kLength, sigma[ind], seed_test_sequence);
    SimpleSeq *seq1 = new SimpleSeq(array);
    SimpleSeqAccess *seq2 = new SimpleSeqAccess(array);
    TestSequence(seq1, seq2);
  }
}

TEST(WaveletTreeNoPtrs, AgainsSimpleSeq) {
  for (cds_word ind = 0; ind < sigmas; ind++) {
    Array *array = CreateRandomSequence(kLength, sigma[ind], seed_test_sequence);
    SimpleSeq *seq1 = new SimpleSeq(array);
    WaveletTreeNoPtrs *seq2 = new WaveletTreeNoPtrs(array, new BitSequenceBuilderOneLevelRank(20), new MapperNone());
    TestSequence(seq1, seq2);
  }
}
