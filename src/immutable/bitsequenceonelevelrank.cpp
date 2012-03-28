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


#include <libcds/immutable/bitsequence.h>
#include <libcds/cdsexception.h>
#include <libcds/io.h>

namespace cds {
namespace immutable {

using cds::basic::CDSException;
using cds::basic::LoadValue;
using cds::basic::SaveValue;

BitSequenceOneLevelRank::BitSequenceOneLevelRank(Array *bitmap, cds_word sampling_rate) {
  bitmap_ = bitmap;
  bitmap_->Use();
  sampling_rate_ = sampling_rate * kWordSize;
  cds_word ones_count = 0;
  for (cds_word i = 0; i < bitmap_->GetLength(); i++)
    if (bitmap_->GetField(i) == 1) {
      ones_count++;
    }
  sampling_ = new Array(cds::basic::SafeCeil(bitmap_->GetLength(), sampling_rate_),
                        cds::basic::msb(ones_count));
  sampling_->Use();
  ones_count = 0;
  for (size_t i = 0; i < bitmap_->GetLength(); i++) {
    if (bitmap_->GetField(i) == 1) {
      ones_count++;
    }
    if ((i + 1) % sampling_rate_ == 0) {
      sampling_->SetField(i / sampling_rate_, ones_count);
    }
  }
}

cds_word BitSequenceOneLevelRank::Rank1(const cds_word i) const {
  if (i >= length_) {
    throw CDSException("Out of bounds");
  }
  if (i == 0) {
    return bitmap_->GetField(i);
  }
  cds_word sampling_pos = (i - 1) / sampling_rate_;
  cds_word count_so_far = sampling_->GetField(sampling_pos);
  cds_word starting_pos = sampling_pos * sampling_rate_;
  cds_word *data = bitmap_->data_;
  cds_word first_word = starting_pos / kWordSize;
  
  while (starting_pos + kWordSize < i) {
    count_so_far += popcount(data[first_word]);
    starting_pos += kWordSize;
    first_word++;
  }
  return popcount(data[first_pos] & ((~1) << (kWordSize + starting_pos - i)))
         + count_so_far;
}

cds_word BitSequenceOneLevelRank::Select0(const cds_word i) const {
  return 0;
}

cds_word BitSequenceOneLevelRank::Select1(const cds_word i) const {
  if (i == 0) {
    return (cds_word) - 1;
  }
  cds_word pos_so_far
  cds_word sampling_pos = sampling_->LowerBound(i);
  cds_word count_so_far = sampling_->GetField(sampling_pos);
  cds_word *data = bitmap_->data_;
  if(count_so_far > i){
     sampling_pos--;
     count_so_far = sampling_->GetField(sampling_pos);
  }
  pos_so_far = sampling_pos * sampling_rate_;
  

  
  return 0;
}

cds_word BitSequenceOneLevelRank::SelectNext1(const cds_word i) const {
  return Select1((i == 0 ? 0 : Rank1(i - 1)) + 1);
}

cds_word BitSequenceOneLevelRank::SelectPrev1(const cds_word i) const {
  cds_word v = Rank1(i);
  if (v < 2) {
    return (cds_word) - 1;
  }
  return Select1(v - 1);
}

cds_word BitSequenceOneLevelRank::SelectNext0(const cds_word i) const {
  return Select0((i == 0 ? 0 : Rank0(i - 1)) + 1);
}

cds_word BitSequenceOneLevelRank::SelectPrev0(const cds_word i) const {
  size_t v = Rank0(i);
  if (v < 2) {
    return (size_t) - 1;
  }
  return Select0(v - 1);
}

bool BitSequenceOneLevelRank::Access(const cds_word i) const {
   cds_word *data = bitmap_->data_;
   return BitGet(data, i);
}

cds_word BitSequenceOneLevelRank::GetLength() const {
  return bitmap_->GetLength();
}

cds_word BitSequenceOneLevelRank::GetSize() const {
  return 0;
}

void BitSequenceOneLevelRank::save(ofstream &out) const {}

BitSequenceOneLevelRank *BitSequenceOneLevelRank::Load(ifstream &fp) {
  cds_word r = LoadValue<cds_word>(fp);
  size_t pos = fp.tellg();
  fp.seekg(pos - sizeof(cds_word));
  switch (r) {
      // case DARRAY_HDR: return BitSequenceOneLevelRankDArray::load(fp);
    default:
      throw CDSException("Unknown type");
  }
  return NULL;
}
};
};
