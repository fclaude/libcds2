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

using std::istream;
using std::ostream;

using cds::basic::Array;
using cds::basic::ArrayTpl;

Array *Array::Create(cds_word *A, cds_word i, cds_word j, cds_word bpe) {
  cds_word max_value = 0;
  for (cds_word k = i; k <= j; k++) {
    max_value = max(max_value, A[k]);
  }
  Array *ret = Create(j - i + 1, max(msb(max_value), bpe));
  for (cds_word k = i; k <= j; k++) {
    assert(A[k] <= max_value);
    ret->SetField(k - i, A[k]);
  }
  return ret;
}

template <cds_word bpe> void ArrayTpl<bpe>::Save(ostream &out) const {
  SaveValue(out, bpe);
  SaveValue(out, length_);
  cds_word uint_length = WordsLength(length_, bpe);
  SaveValue(out, data_, uint_length);
}

template <cds_word bpe> ArrayTpl<bpe>::ArrayTpl(istream &input) {
  length_ = LoadValue<cds_word>(input);
  cds_word uint_length = WordsLength(length_, bpe);
  data_ = LoadValue<cds_word>(input, uint_length);
}

template <cds_word bpe> ArrayTpl<bpe>* ArrayTpl<bpe>::Load(istream &input) {
  cds_word id = LoadValue<cds_word>(input);
  if (id != bpe) {
    std::cout << "Returning null: " << bpe << " - " << id << std::endl;
    return NULL;
  }
  return new ArrayTpl<bpe>(input);
}

template <cds_word bpe> ArrayTpl<bpe>::ArrayTpl(cds_word n) {
  length_ = n;
  InitData();
}

template <cds_word bpe> ArrayTpl<bpe>::ArrayTpl(cds_word *A, cds_word i, cds_word j) {
  length_ = j - i + 1;
  InitData();
  for (cds_word k = i; k <= j; k++) {
    SetField(k - i, A[k]);
  }
}

template <cds_word bpe> cds_word ArrayTpl<bpe>::GetSize() const {
  return sizeof(cds_word) * WordsLength(length_, bpe)
         + sizeof(cds_word) * 4
         + sizeof(cds_word *);
}

template <cds_word bpe> cds_word ArrayTpl<bpe>::GetLength() const {
  return length_;
}


template <cds_word bpe> ArrayTpl<bpe>::~ArrayTpl() {
  delete [] data_;
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
    if (GetField(ini + sum) >= value) {
      return LowerBound(value, ini, ini + sum);
    }
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
  cds_word uint_length = WordsLength(length_, bpe);
  data_ = new cds_word[uint_length];
  for (cds_word i = 0; i < uint_length; i++) {
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
  cds::basic::SetField(data_, bpe, position, v);
  return v;
}

Array *Array::Load(istream &input) {
  cds_word id = LoadValue<cds_word>(input);
  size_t pos = input.tellg();
  input.seekg(pos - sizeof(cds_word));
  switch(id) {
    case 0: return ArrayTpl<0>::Load(input);
    case 1: return ArrayTpl<1>::Load(input);
    case 2: return ArrayTpl<2>::Load(input);
    case 3: return ArrayTpl<3>::Load(input);
    case 4: return ArrayTpl<4>::Load(input);
    case 5: return ArrayTpl<5>::Load(input);
    case 6: return ArrayTpl<6>::Load(input);
    case 7: return ArrayTpl<7>::Load(input);
    case 8: return ArrayTpl<8>::Load(input);
    case 9: return ArrayTpl<9>::Load(input);
    case 10: return ArrayTpl<10>::Load(input);
    case 11: return ArrayTpl<11>::Load(input);
    case 12: return ArrayTpl<12>::Load(input);
    case 13: return ArrayTpl<13>::Load(input);
    case 14: return ArrayTpl<14>::Load(input);
    case 15: return ArrayTpl<15>::Load(input);
    case 16: return ArrayTpl<16>::Load(input);
    case 17: return ArrayTpl<17>::Load(input);
    case 18: return ArrayTpl<18>::Load(input);
    case 19: return ArrayTpl<19>::Load(input);
    case 20: return ArrayTpl<20>::Load(input);
    case 21: return ArrayTpl<21>::Load(input);
    case 22: return ArrayTpl<22>::Load(input);
    case 23: return ArrayTpl<23>::Load(input);
    case 24: return ArrayTpl<24>::Load(input);
    case 25: return ArrayTpl<25>::Load(input);
    case 26: return ArrayTpl<26>::Load(input);
    case 27: return ArrayTpl<27>::Load(input);
    case 28: return ArrayTpl<28>::Load(input);
    case 29: return ArrayTpl<29>::Load(input);
    case 30: return ArrayTpl<30>::Load(input);
    case 31: return ArrayTpl<31>::Load(input);
    case 32: return ArrayTpl<32>::Load(input);
    case 33: return ArrayTpl<33>::Load(input);
    case 34: return ArrayTpl<34>::Load(input);
    case 35: return ArrayTpl<35>::Load(input);
    case 36: return ArrayTpl<36>::Load(input);
    case 37: return ArrayTpl<37>::Load(input);
    case 38: return ArrayTpl<38>::Load(input);
    case 39: return ArrayTpl<39>::Load(input);
    case 40: return ArrayTpl<40>::Load(input);
    case 41: return ArrayTpl<41>::Load(input);
    case 42: return ArrayTpl<42>::Load(input);
    case 43: return ArrayTpl<43>::Load(input);
    case 44: return ArrayTpl<44>::Load(input);
    case 45: return ArrayTpl<45>::Load(input);
    case 46: return ArrayTpl<46>::Load(input);
    case 47: return ArrayTpl<47>::Load(input);
    case 48: return ArrayTpl<48>::Load(input);
    case 49: return ArrayTpl<49>::Load(input);
    case 50: return ArrayTpl<50>::Load(input);
    case 51: return ArrayTpl<51>::Load(input);
    case 52: return ArrayTpl<52>::Load(input);
    case 53: return ArrayTpl<53>::Load(input);
    case 54: return ArrayTpl<54>::Load(input);
    case 55: return ArrayTpl<55>::Load(input);
    case 56: return ArrayTpl<56>::Load(input);
    case 57: return ArrayTpl<57>::Load(input);
  }
  return NULL;
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
};
};
