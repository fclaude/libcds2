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

#include <libcds/immutable/bitsequenceseq.h>

namespace cds {
namespace immutable {

using cds::basic::cds_word;
using cds::basic::Array;

BitSequenceSeq::BitSequenceSeq(Array *array) {
  array_ = array;
}

cds_word BitSequenceSeq::Rank1(const cds_word i) const {
  cds_word cnt = 0;
  for (cds_word k = 0; k <= i; k++) {
    cnt += array_->GetField(k);
  }
  return cnt;
}

cds_word BitSequenceSeq::Rank0(const cds_word i) const {
  cds_word cnt = 0;
  for (cds_word k = 0; k <= i; k++) {
    cnt += 1 - array_->GetField(k);
  }
  return cnt;
}

cds_word BitSequenceSeq::Rank0(const cds_word i, bool *access) const {
  *access = Access(i);
  return Rank0(i);
}

cds_word BitSequenceSeq::Rank1(const cds_word i, bool *access) const {
  *access = Access(i);
  return Rank1(i);
}

cds_word BitSequenceSeq::Select1(const cds_word i) const {
  if (i == 0) {
    return (cds_word) - 1;
  }
  cds_word cnt = 0;
  for (cds_word k = 0; k < array_->GetLength(); k++) {
    cnt += array_->GetField(k);
    if (cnt == i) {
      return k;
    }
  }
  return array_->GetLength();
}

cds_word BitSequenceSeq::SelectNext1(const cds_word i) const {
  for (cds_word k = i; k < array_->GetLength(); k++) {
    if (1 == array_->GetField(k)) {
      return k;
    }
  }
  return array_->GetLength();
}

cds_word BitSequenceSeq::SelectPrev1(const cds_word i) const {
  for (cds_word k = i; k > 0; k--) {
    if (1 == array_->GetField(k)) {
      return k;
    }
  }
  if (array_->GetField(0) == 1) {
    return 0;
  } else {
    return (cds_word) - 1;
  }
}

cds_word BitSequenceSeq::Select0(const cds_word i) const {
  if (i == 0) {
    return (cds_word) - 1;
  }
  cds_word cnt = 0;
  for (cds_word k = 0; k < array_->GetLength(); k++) {
    cnt += 1 - array_->GetField(k);
    if (cnt == i) {
      return k;
    }
  }
  return array_->GetLength();
}

cds_word BitSequenceSeq::SelectNext0(const cds_word i) const {
  for (cds_word k = i; k < array_->GetLength(); k++) {
    if (0 == array_->GetField(k)) {
      return k;
    }
  }
  return array_->GetLength();
}

cds_word BitSequenceSeq::SelectPrev0(const cds_word i) const {
  for (cds_word k = i; k > 0; k--) {
    if (0 == array_->GetField(k)) {
      return k;
    }
  }
  return (array_->GetField(0) == 1) ? ((cds_word) - 1) : 0;
}

cds_word BitSequenceSeq::CountOnes() const {
  return Rank1(array_->GetLength() - 1);
}

cds_word BitSequenceSeq::CountZeros() const {
  return Rank0(array_->GetLength() - 1);
}

cds_word BitSequenceSeq::GetSize() const {
  return array_->GetSize() + sizeof(*this);
}

void BitSequenceSeq::Save(ofstream &fp) const {
}

cds_word BitSequenceSeq::GetLength() const {
  return array_->GetLength();
}

bool BitSequenceSeq::Access(const cds_word i) const {
  return array_->GetField(i) > 0;
}
};
};
