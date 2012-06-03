/********************************************************************************
Copyright (c) 2012, Francisco Claude and Rodrigo Canovas.
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


#include <libcds/immutable/bitsequenceonelevelrank.h>
#include <libcds/cdsexception.h>
#include <libcds/io.h>

#include <algorithm>

namespace cds {
namespace immutable {

using cds::basic::CDSException;
using cds::basic::LoadValue;
using cds::basic::SaveValue;
using cds::basic::kWordSize;

BitSequenceOneLevelRank::BitSequenceOneLevelRank(Array *bitmap, cds_word sampling_rate) {
  bitmap_ = reinterpret_cast<ArrayTpl<1>*>(bitmap);
  bitmap_->Use();
  sampling_rate_ = sampling_rate * kWordSize;
  cds_word ones_count = 0;
  for (cds_word i = 0; i < bitmap_->GetLength(); i++)
    if (bitmap_->GetField(i) == 1) {
      ones_count++;
    }
  sampling_ = Array::Create(cds::basic::SafeCeil(bitmap_->GetLength(), sampling_rate_),
                            cds::basic::msb(ones_count));
  sampling_->Use();
  ones_count = 0;
  for (size_t i = 0; i < bitmap_->GetLength(); i++) {
    if (i % sampling_rate_ == 0) {
      sampling_->SetField(i / sampling_rate_, ones_count);
    }
    if (bitmap_->GetField(i) == 1) {
      ones_count++;
    }
  }
}

bool BitSequenceOneLevelRank::Access(const cds_word i, cds_word *r) const {
  assert(i < GetLength());
  if (i >= bitmap_->GetLength()) {
    throw CDSException("Rank1 out of bounds");
  }

  if (i == 0) {
    bool ret = bitmap_->GetField(i);
    *r = 1;
    return ret;
  }

  cds_word sampling_pos = i / sampling_rate_;
  cds_word count_so_far = sampling_->GetField(sampling_pos);
  cds_word starting_pos = sampling_pos * sampling_rate_;
  cds_word *data = bitmap_->data_;
  cds_word first_word = starting_pos / kWordSize;

  while (starting_pos + kWordSize <= i) {
    count_so_far += popcount(data[first_word]);
    starting_pos += kWordSize;
    first_word++;
  }

  cds_word pos_in_word = i - starting_pos;
  *r = popcount(data[first_word] << (kWordSize - 1 - pos_in_word))
       + count_so_far;
  if (1 & (data[first_word] >> pos_in_word)) {
    return true;
  }

  *r = i - *r + 1;
  return false;
}

cds_word BitSequenceOneLevelRank::Rank1(const cds_word i) const {
  assert(i < GetLength());
  if (i >= bitmap_->GetLength()) {
    throw CDSException("Rank1 out of bounds");
  }

  if (i == 0) {
    return bitmap_->GetField(i);
  }

  cds_word sampling_pos = i / sampling_rate_;
  cds_word count_so_far = sampling_->GetField(sampling_pos);
  cds_word starting_pos = sampling_pos * sampling_rate_;
  cds_word *data = bitmap_->data_;
  cds_word first_word = starting_pos / kWordSize;

  while (starting_pos + kWordSize <= i) {
    count_so_far += popcount(data[first_word]);
    starting_pos += kWordSize;
    first_word++;
  }

  return popcount(data[first_word] << (kWordSize - i + starting_pos - 1))
         + count_so_far;
}

cds_word BitSequenceOneLevelRank::Rank1(const cds_word i, bool *a) const {
  assert(i < GetLength());
  if (i >= bitmap_->GetLength()) {
    throw CDSException("Rank1 out of bounds");
  }

  if (i == 0) {
    *a = bitmap_->GetField(i);
    return *a;
  }

  cds_word sampling_pos = i / sampling_rate_;
  cds_word count_so_far = sampling_->GetField(sampling_pos);
  cds_word starting_pos = sampling_pos * sampling_rate_;
  cds_word *data = bitmap_->data_;
  cds_word first_word = starting_pos / kWordSize;

  while (starting_pos + kWordSize <= i) {
    count_so_far += popcount(data[first_word]);
    starting_pos += kWordSize;
    first_word++;
  }

  cds_word pos_in_word = i - starting_pos;
  *a = 1 & (data[first_word] >> pos_in_word);
  return popcount(data[first_word] << (kWordSize - 1 - pos_in_word))
         + count_so_far;
}

cds_word BitSequenceOneLevelRank::Rank0(const cds_word i, bool *a) const {
  return i + 1 - Rank1(i, a);
}

cds_word BitSequenceOneLevelRank::Select0(const cds_word i) const {
  if (i == 0) {
    return static_cast<cds_word>(-1);
  }

  cds_word ini = 0, fin = sampling_->GetLength();
  cds_word count, step;
  count = fin - ini;

  while (count > 0) {
    step = count / 2;
    cds_word pos = ini + step;
    if (pos - sampling_->GetField(pos) < i) {
      ini = pos + 1;
      count -= step + 1;
    } else {
      count = step;
    }
  }

  cds_word last_word = cds::basic::SafeCeil(GetLength(), kWordSize);
  cds_word sampling_pos = ini - 1;
  cds_word count_so_far = sampling_->GetField(sampling_pos);
  cds_word *data = bitmap_->data_;
  cds_word pos_so_far = sampling_pos * sampling_rate_;
  cds_word first_word = pos_so_far / kWordSize;
  cds_word zeroes = popcount(~data[first_word]);

  while ((count_so_far + zeroes) < i && first_word < last_word) {
    count_so_far += zeroes;
    first_word++;
    zeroes = popcount(~data[first_word]);
  }

  if (i - count_so_far > popcount(~data[first_word])) {
    return GetLength();
  }

  return first_word * kWordSize + cds::basic::select(~data[first_word], i - count_so_far);
}

cds_word BitSequenceOneLevelRank::Select1(const cds_word i) const {
  if (i == 0) {
    return static_cast<cds_word>(-1);
  }

  // We find the word that contains the answer
  cds_word last_word = cds::basic::SafeCeil(GetLength(), kWordSize);
  cds_word sampling_pos = sampling_->LowerBound(i) - 1;
  cds_word count_so_far = sampling_->GetField(sampling_pos);
  cds_word *data = bitmap_->data_;
  cds_word pos_so_far = sampling_pos * sampling_rate_;
  cds_word first_word = pos_so_far / kWordSize;
  cds_word ones = popcount(data[first_word]);

  while ((count_so_far + ones) < i && first_word < last_word + 1) {
    count_so_far += ones;
    first_word++;
    ones = popcount(data[first_word]);
  }

  if (i - count_so_far > popcount(data[first_word])) {
    return GetLength();
  }

  return first_word * kWordSize + cds::basic::select(data[first_word], i - count_so_far);
}

cds_word BitSequenceOneLevelRank::SelectNext1(const cds_word i) const {
  cds_word *data = bitmap_->data_;
  cds_word tmp_word = (data[i / kWordSize] >> (i % kWordSize));
  if (popcount(tmp_word)) {
    return i + cds::basic::select(tmp_word, 1);
  }

  cds_word last_word = cds::basic::SafeCeil(GetLength(), kWordSize);
  cds_word first_word = i / kWordSize + 1;
  cds_word sampling_pos = i / sampling_rate_ + 1;
  cds_word seq_search_limit = std::min(sampling_pos * sampling_rate_ / kWordSize, last_word + 1);

  while (first_word < seq_search_limit) {
    if (data[first_word]) {
      return first_word * kWordSize + cds::basic::select(data[first_word], 1);
    }
    first_word++;
  }

  sampling_pos--;
  cds_word new_i = sampling_->GetField(sampling_pos) + 1;
  sampling_pos = sampling_->LowerBoundExp(new_i, sampling_pos, sampling_->GetLength());
  cds_word pos_so_far = sampling_pos * sampling_rate_;
  first_word = pos_so_far / kWordSize;

  cds_word count_so_far = new_i - 1;
  cds_word ones = popcount(data[first_word]);

  while ((count_so_far + ones) < new_i && first_word < last_word + 1) {
    count_so_far += ones;
    first_word++;
    ones = popcount(data[first_word]);
  }

  if (new_i - count_so_far > popcount(data[first_word])) {
    return GetLength();
  }

  return first_word * kWordSize + cds::basic::select(data[first_word], new_i - count_so_far);
}

bool BitSequenceOneLevelRank::Access(const cds_word i) const {
  return bitmap_->GetField(i);
}

cds_word BitSequenceOneLevelRank::GetLength() const {
  return bitmap_->GetLength();
}

cds_word BitSequenceOneLevelRank::GetSize() const {
  return sizeof(BitSequenceOneLevelRank) + bitmap_->GetSize() + sampling_->GetSize();
}

void BitSequenceOneLevelRank::Save(ostream &out) const {
  SaveValue(out, kBitSequenceOneLevelRankID);
  bitmap_->Save(out);
  sampling_->Save(out);
  SaveValue(out, length_);
  SaveValue(out, sampling_rate_);
}

BitSequenceOneLevelRank *BitSequenceOneLevelRank::Load(istream &fp) {
  cds_word r = LoadValue<cds_word>(fp);
  if (r != kBitSequenceOneLevelRankID) {
    assert(false);
    return NULL;
  }
  BitSequenceOneLevelRank *ret = new BitSequenceOneLevelRank();
  ret->bitmap_ = ArrayTpl<1>::Load(fp);
  ret->sampling_ = Array::Load(fp);
  ret->length_ = LoadValue<cds_word>(fp);
  ret->sampling_rate_ = LoadValue<cds_word>(fp);
  return ret;
}
};
};
