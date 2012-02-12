/**
 *    array.cpp
 *    Copyright (C) 2011  Francisco Claude F.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
