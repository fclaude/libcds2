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


#include <libcds/io.h>
#include <libcds/array.h>

#include <algorithm>

namespace cds {
namespace basic {

using std::max;

Array::Array(ifstream &input) {
  length_ = LoadValue<cds_word>(input);
  max_value_ = LoadValue<cds_word>(input);
  bits_per_item_ = LoadValue<cds_word>(input);
  uint_length_ = LoadValue<cds_word>(input);
  data_ = LoadValue<cds_word>(input, uint_length_);
  Use();
}

Array::Array(cds_word n, cds_word bpe) {
  length_ = n;
  max_value_ = ((cds_word)1 << bpe) - 1;
  InitData();
  Use();
}

/*Array::Array(cds_word * A, cds_word n, cds_word bpe) {
      maxValue = 0;
      if(bpe==0) {
          for(cds_word k=0;k<n;k++)
              maxValue = max(maxValue,A[k]);
      }
      else {
  maxValue = ((cds_word)1<<(bpe))-1;
      }
      length = n;
      initData();
assert(bpe==0 || bitsPerItem==bpe);
for(cds_word k=0;k<n;k++) {
assert(A[k] <= maxValue);
setField(k, A[k]);
}
use();
}*/

Array::Array(cds_word *A, cds_word i, cds_word j, cds_word bpe) {
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
  assert(bpe == 0 || bits_per_item_ == bpe);
  for (cds_word k = i; k <= j; k++) {
    assert(A[k] <= max_value_);
    SetField(k - i, A[k]);
  }
  Use();
}

Array::~Array() {
  //std::cout << "Bye bye" << std::endl;
  delete [] data_;
}

void Array::Save(ofstream &out) const {
  SaveValue(out, length_);
  SaveValue(out, max_value_);
  SaveValue(out, bits_per_item_);
  SaveValue(out, uint_length_);
  SaveValue(out, data_, uint_length_);
}

cds_word Array::GetMax() const {
  cds_word max_value = 0;                     // default max
  for (cds_word i = 0; i < length_; i++) {
    max_value = max(max_value, GetField(i));
  }
  return max_value;
}

// Implementation based on the STL one.
cds_word Array::LowerBound(cds_word value, cds_word ini, cds_word fin) const {
  cds_word count, step;
  count = fin - ini;

  while (count > 0) {
    step = count / 2; 
    cds_word pos = ini + step;
    if (GetField(pos) < value) {
      ini = pos + 1;
      count -= step + 1; 
    } else count = step;
  }
  return ini;
}

cds_word Array::LowerBound(cds_word value) const {
  return LowerBound(value, 0, length_);
}

// Also based on the STL version.
cds_word Array::UpperBound(cds_word value, cds_word ini, cds_word fin) const {
  cds_word count, step;
  count = fin - ini;

  while (count > 0) {
    step = count / 2;
    cds_word pos = ini + step;
    if (!(value < GetField(pos))) {
      ini = pos + 1;
      count -= step + 1;
    } else count = step;
  }
  return ini;
}

cds_word Array::UpperBound(cds_word value) const {
  return UpperBound(value, 0, length_);  
}

cds_word Array::BinarySearch(cds_word value, cds_word ini, cds_word fin) const {
  cds_word pos = LowerBound(value, ini, fin);
  if (pos != length_ && !(value < GetField(pos))) 
    return pos;
  else
    return length_;
}

cds_word Array::BinarySearch(cds_word value) const {
  return BinarySearch(value, 0, length_);
}

void Array::InitData() {
  bits_per_item_ = msb(max_value_);
  uint_length_ = WordsLength(length_, bits_per_item_);
  data_ = new cds_word[uint_length_];
  for (cds_word i = 0; i < uint_length_; i++) {
    data_[i] = 0;
  }
}
};
};
