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
#include "./test_bitsequence_utils.h"

using cds::immutable::BitSequence;
using cds::immutable::BitSequenceSeq;
using cds::basic::Array;
using cds::basic::cds_word;
using std::ostream;

class BitSequenceRank0: public BitSequence {
  public:
    explicit BitSequenceRank0(Array *_a) {
      bs_ = new BitSequenceSeq(_a);
      bs_->Use();
    }

    virtual ~BitSequenceRank0() {
      bs_->Unuse();
    }

    virtual cds_word GetLength() const {
      return bs_->GetLength();
    }

    virtual cds_word Rank0(const cds_word i) const {
      return bs_->Rank0(i);
    }

    virtual cds_word Rank0(const cds_word i, bool *access) const {
      *access = Access(i);
      return Rank0(i);
    }

    virtual cds_word Rank1(const cds_word i) const {
      return i - Rank0(i) + 1;
    }

    virtual cds_word Rank1(const cds_word i, bool *access) const {
      *access = Access(i);
      return Rank1(i);
    }

    virtual void Save(ostream &fp) const {
    }

    virtual cds_word GetSize() const {
      return bs_->GetSize() + sizeof(*this);
    }

    virtual cds_word CountZeros() const {
      return bs_->CountZeros();
    }

  protected:
    BitSequence *bs_;
};



TEST(BitSequence, SupportingRank0) {
  const cds_word kBitmapLength = 10000;
  const cds_word kOnes = kBitmapLength / 4;
  unsigned int seed = 101;
  Array *a = CreateRandomBitmap(kBitmapLength, kOnes, seed);
  BitSequenceSeq *seq_bitseq = new BitSequenceSeq(a);
  seq_bitseq->Use();
  BitSequenceRank0 *bs = new BitSequenceRank0(a);
  bs->Use();
  TestBitSequence(seq_bitseq, bs);
  a->Unuse();
  bs->Unuse();
  seq_bitseq->Unuse();
}
