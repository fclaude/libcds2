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


#include <libcds/array.h>
#include <libcds/libcds.h>

#include <fstream>
#include <algorithm>

namespace cds {
namespace basic {

using std::ifstream;
using std::ofstream;

using cds::basic::Array;
using cds::basic::ArrayTpl;

Array *Array::Load(ifstream &input) {
  return NULL;
}

Array *Array::Create(cds_word *A, cds_word i, cds_word j, cds_word bpe) {
  cds_word max_value = 0;
  if (bpe == 0) {
    for (cds_word k = i; k <= j; k++) {
      max_value = max(max_value, A[k]);
    }
  } else {
    max_value = ((cds_word)1 << bpe) - 1;
  }
  Array *ret = Create(j - i + 1, msb(max_value));
  for (cds_word k = i; k <= j; k++) {
    assert(A[k] <= max_value);
    ret->SetField(k - i, A[k]);
  }
  return ret;
}

Array *Array::Create(cds_word n, cds_word bpe) {
  switch (bpe) {
    case 0:
      return new ArrayTpl<0>(n);
    case 1:
      return new ArrayTpl<1>(n);
    case 2:
      return new ArrayTpl<2>(n);
    case 3:
      return new ArrayTpl<3>(n);
    case 4:
      return new ArrayTpl<4>(n);
    case 5:
      return new ArrayTpl<5>(n);
    case 6:
      return new ArrayTpl<6>(n);
    case 7:
      return new ArrayTpl<7>(n);
    case 8:
      return new ArrayTpl<8>(n);
    case 9:
      return new ArrayTpl<9>(n);
    case 10:
      return new ArrayTpl<10>(n);
    case 11:
      return new ArrayTpl<11>(n);
    case 12:
      return new ArrayTpl<12>(n);
    case 13:
      return new ArrayTpl<13>(n);
    case 14:
      return new ArrayTpl<14>(n);
    case 15:
      return new ArrayTpl<15>(n);
    case 16:
      return new ArrayTpl<16>(n);
    case 17:
      return new ArrayTpl<17>(n);
    case 18:
      return new ArrayTpl<18>(n);
    case 19:
      return new ArrayTpl<19>(n);
    case 20:
      return new ArrayTpl<20>(n);
    case 21:
      return new ArrayTpl<21>(n);
    case 22:
      return new ArrayTpl<22>(n);
    case 23:
      return new ArrayTpl<23>(n);
    case 24:
      return new ArrayTpl<24>(n);
    case 25:
      return new ArrayTpl<25>(n);
    case 26:
      return new ArrayTpl<26>(n);
    case 27:
      return new ArrayTpl<27>(n);
    case 28:
      return new ArrayTpl<28>(n);
    case 29:
      return new ArrayTpl<29>(n);
    case 30:
      return new ArrayTpl<30>(n);
    case 31:
      return new ArrayTpl<31>(n);
    case 32:
      return new ArrayTpl<32>(n);
    case 33:
      return new ArrayTpl<33>(n);
    case 34:
      return new ArrayTpl<34>(n);
    case 35:
      return new ArrayTpl<35>(n);
    case 36:
      return new ArrayTpl<36>(n);
    case 37:
      return new ArrayTpl<37>(n);
    case 38:
      return new ArrayTpl<38>(n);
    case 39:
      return new ArrayTpl<39>(n);
    case 40:
      return new ArrayTpl<40>(n);
    case 41:
      return new ArrayTpl<41>(n);
    case 42:
      return new ArrayTpl<42>(n);
    case 43:
      return new ArrayTpl<43>(n);
    case 44:
      return new ArrayTpl<44>(n);
    case 45:
      return new ArrayTpl<45>(n);
    case 46:
      return new ArrayTpl<46>(n);
    case 47:
      return new ArrayTpl<47>(n);
    case 48:
      return new ArrayTpl<48>(n);
    case 49:
      return new ArrayTpl<49>(n);
    case 50:
      return new ArrayTpl<50>(n);
    case 51:
      return new ArrayTpl<51>(n);
    case 52:
      return new ArrayTpl<52>(n);
    case 53:
      return new ArrayTpl<53>(n);
    case 54:
      return new ArrayTpl<54>(n);
    case 55:
      return new ArrayTpl<55>(n);
    case 56:
      return new ArrayTpl<56>(n);
    case 57:
      return new ArrayTpl<57>(n);
    case 58:
      return new ArrayTpl<58>(n);
  }
  return NULL;
}

template <cds_word bpe> ArrayTpl<bpe>::ArrayTpl(ifstream &input) {
  length_ = LoadValue<cds_word>(input);
  max_value_ = LoadValue<cds_word>(input);
  uint_length_ = LoadValue<cds_word>(input);
  data_ = LoadValue<cds_word>(input, uint_length_);
  // Use();
}

template <cds_word bpe> ArrayTpl<bpe>::ArrayTpl(cds_word n) {
  length_ = n;
  max_value_ = ((cds_word)1 << bpe) - 1;
  InitData();
  // Use();
}

template <cds_word bpe> ArrayTpl<bpe>::ArrayTpl(cds_word *A, cds_word i, cds_word j) {
  max_value_ = 0;
  if (bpe == 0) {
    for (cds_word k = i; k <= j; k++) {
      max_value_ = max(max_value_, A[k]);
    }
  } else {
    max_value_ = ((cds_word)1 << bpe) - 1;
  }
  length_ = j - i + 1;
  InitData();
  for (cds_word k = i; k <= j; k++) {
    assert(A[k] <= max_value_);
    SetField(k - i, A[k]);
  }
  // Use();
}

template <cds_word bpe> cds_word ArrayTpl<bpe>::GetSize() const {
  return sizeof(cds_word) * uint_length_
         + sizeof(cds_word) * 4
         + sizeof(cds_word *);
}

template <cds_word bpe> cds_word ArrayTpl<bpe>::GetLength() const {
  return length_;
}


template <cds_word bpe> ArrayTpl<bpe>::~ArrayTpl() {
  delete [] data_;
}

template <cds_word bpe> void ArrayTpl<bpe>::Save(ofstream &out) const {
  SaveValue(out, bpe);
  SaveValue(out, length_);
  SaveValue(out, max_value_);
  SaveValue(out, uint_length_);
  SaveValue(out, data_, uint_length_);
}

template <cds_word bpe> cds_word ArrayTpl<bpe>::GetMax() const {
  cds_word max_value = 0;  // default max
  for (cds_word i = 0; i < length_; i++) {
    max_value = max(max_value, GetField(i));
  }
  return max_value;
}

// Implementation based on the STL lower_bound implementation.
template <cds_word bpe> cds_word ArrayTpl<bpe>::LowerBound(cds_word value, cds_word ini, cds_word fin) const {
  assert(ini <= fin);
  assert(fin <= length_);
  cds_word count, step;
  count = fin - ini;

  while (count > 0) {
    step = count / 2;
    cds_word pos = ini + step;
    if (GetField(pos) < value) {
      ini = pos + 1;
      count -= step + 1;
    } else {
      count = step;
    }
  }
  return ini;
}

template <cds_word bpe> cds_word ArrayTpl<bpe>::LowerBound(cds_word value) const {
  return LowerBound(value, 0, length_);
}

template <cds_word bpe> cds_word ArrayTpl<bpe>::LowerBoundExp(cds_word value, cds_word ini, cds_word fin) const {
  cds_word sum = 1;
  while (ini + sum < fin) {
    if (GetField(ini + sum) >= value)
      return LowerBound(value, ini, ini + sum);
    ini += sum;
    sum *= 2;
  }
  return LowerBound(value, ini, fin);
}

template <cds_word bpe> cds_word ArrayTpl<bpe>::LowerBoundExp(cds_word value) const {
  return LowerBoundExp(value, 0, length_);
}

// Also based on the STL version.
template <cds_word bpe> cds_word ArrayTpl<bpe>::UpperBound(cds_word value, cds_word ini, cds_word fin) const {
  assert(ini <= fin);
  assert(fin <= length_);
  cds_word count, step;
  count = fin - ini;

  while (count > 0) {
    step = count / 2;
    cds_word pos = ini + step;
    if (!(value < GetField(pos))) {
      ini = pos + 1;
      count -= step + 1;
    } else {
      count = step;
    }
  }
  return ini;
}

template <cds_word bpe> cds_word ArrayTpl<bpe>::UpperBound(cds_word value) const {
  return UpperBound(value, 0, length_);
}

template <cds_word bpe> cds_word ArrayTpl<bpe>::BinarySearch(cds_word value, cds_word ini, cds_word fin) const {
  assert(ini <= fin);
  assert(fin <= length_);
  cds_word pos = LowerBound(value, ini, fin);
  if (pos != length_ && !(value < GetField(pos))) {
    return pos;
  } else {
    return length_;
  }
}

template <cds_word bpe> cds_word ArrayTpl<bpe>::BinarySearch(cds_word value) const {
  return BinarySearch(value, 0, length_);
}

template <cds_word bpe> void ArrayTpl<bpe>::InitData() {
  uint_length_ = WordsLength(length_, bpe);
  data_ = new cds_word[uint_length_];
  for (cds_word i = 0; i < uint_length_; i++) {
    data_[i] = 0;
  }
}

template <> cds_word ArrayTpl<32>::GetField(const cds_word position) const {
  assert(position < length_);
  return reinterpret_cast<uint32_t *>(data_)[position];
}

template <> cds_word ArrayTpl<16>::GetField(const cds_word position) const {
  assert(position < length_);
  return reinterpret_cast<uint16_t *>(data_)[position];
}

template <> cds_word ArrayTpl<8>::GetField(const cds_word position) const {
  assert(position < length_);
  return reinterpret_cast<uint8_t *>(data_)[position];
}

template <> cds_word ArrayTpl<4>::GetField(const cds_word position) const {
  assert(position < length_);
  return cds::basic::GetField4(data_, position);
}

template <> cds_word ArrayTpl<2>::GetField(const cds_word position) const {
  assert(position < length_);
  return cds::basic::GetField2(data_, position);
}

template <> cds_word ArrayTpl<1>::GetField(const cds_word position) const {
  assert(position < length_);
  if (cds::basic::BitGet(data_, position)) {
    return 1;
  }
  return 0;
}

template <cds_word bpe> cds_word ArrayTpl<bpe>::GetField(const cds_word position) const {
  assert(position < length_);
  return cds::basic::GetField(data_, bpe, position);
}

template <cds_word bpe> cds_word ArrayTpl<bpe>::SetField(const cds_word position, const cds_word v) {
  assert(position < length_);
  assert(v <= max_value_);
  cds::basic::SetField(data_, bpe, position, v);
  return v;
}
};
};
