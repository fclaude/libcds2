/********************************************************************************
Copyright (c) 2012, Francisco Claude
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

#include <libcds/immutable/wavelettreenoptrs.h>
#include <libcds/io.h>

#include <vector>

namespace cds {
namespace immutable {

using cds::basic::LoadValue;
using cds::basic::SaveValue;

using std::vector;

inline cds_word GetStart(cds_word symbol, cds_word mask) {
  return symbol & mask;
}

inline cds_word GetEnd(cds_word symbol, cds_word mask) {
  return GetStart(symbol, mask) + ~mask + 1;
}

inline bool IsSet(cds_word val, cds_word ind, cds_word height) {
  return (val & ((cds_word)1 << (height - ind - 1))) != 0;
}

WaveletTreeNoPtrs::WaveletTreeNoPtrs(const cds::basic::Array *a, BitSequenceBuilder *bmb, Mapper *am) {
  bmb->Use();
  am->Use();
  n_ = a->GetLength();
  am_ = am;
  max_v_ = a->GetMax();
  
  cds_word mapped_max = am_->Map(max_v_);
  height_ = cds::basic::msb(mapped_max);

  vector<cds_word> occ(mapped_max + 2);
  for (cds_word i = 0; i < a->GetLength(); i++) {
    occ[1 + am_->Map(a->GetField(i))]++;
  }

  cds_word missing = 0;
  vector<cds_word> missing_values;
  for (cds_word i = 1; i <= mapped_max + 1; i++) {
    if (occ[i] == 0) {
      missing++;
      missing_values.push_back(i - 1);
      occ[i] = 1;
    }
  }
  for(cds_word i = 1; i <= mapped_max + 1; i++)
      occ[i] += occ[i - 1];
  occ_ = Array::Create(occ);
  occ_->Use();

  Array *new_array = Array::Create(n_ + missing, cds::basic::msb(mapped_max));
  new_array->Use();
  for (cds_word i = 0; i < n_; i++) {
    new_array->SetField(i, am_->Map(a->GetField(i)));
  }
  for (cds_word i = n_; i < n_ + missing; i++) {
    new_array->SetField(i, missing_values[i - n_]);
  }
  
  Array **bitmaps = new Array*[height_];
  for (cds_word i = 0; i < height_; i++) {
    bitmaps[i] = Array::Create(n_ + missing, 1);
    bitmaps[i]->Use();
  }
  BuildLevels(new_array, bitmaps, 0, n_ + missing, 0);

  level_ = new BitSequence*[height_];
  for (cds_word i = 0; i < height_; i++) {
    level_[i] = bmb->Build(bitmaps[i]);
    level_[i]->Use();
    bitmaps[i]->Unuse();
  }

  new_array->Unuse();
  bmb->Unuse();
}

void WaveletTreeNoPtrs::BuildLevels(Array *new_array, Array **bitmaps, cds_word ini, cds_word fin, cds_word level) {
  if (level == height_) return;
  assert (ini < fin || ini == new_array->GetLength());
  
  Array *tmp = Array::Create(fin - ini, height_);
  tmp->Use();
  
  cds_word left = 0, right = 0;
  for (cds_word i = ini; i < fin; i++) {
    if (IsSet(new_array->GetField(i), level, height_)) {
      bitmaps[level]->SetField(i, 1);
    } else {
      right++;
    }
  }
  
  for (cds_word i = ini; i < fin; i++) {
    if (!IsSet(new_array->GetField(i), level, height_)) {
      tmp->SetField(left, new_array->GetField(i));
      left++;
    } else {
      tmp->SetField(right, new_array->GetField(i));
      right++;
    }
  }

  for (cds_word i = ini; i < fin; i++) {
    new_array->SetField(i, tmp->GetField(i - ini));
  }
  tmp->Unuse();

  BuildLevels(new_array, bitmaps, ini, ini + left, level + 1);
  BuildLevels(new_array, bitmaps, ini + left, ini + right, level + 1);
}


WaveletTreeNoPtrs::~WaveletTreeNoPtrs() {
  for (cds_word i = 0; i < height_; i++) {
    level_[i]->Unuse();
  }
  occ_->Unuse();
  am_->Unuse();
}

cds_word WaveletTreeNoPtrs::Rank(cds_word symbol, cds_word pos) const {
  symbol = am_->Map(symbol);;

  cds_word start = 0;
  cds_word count = 0;
  
  for(cds_word level = 0; level < height_; level++) {
    
    cds_word masked = (symbol >> (height_ - level - 1)) << (height_ - level - 1);
    
    cds_word before = 0;
    if (start > 0)
      before = level_[level]->Rank1(start - 1);
    
    if (IsSet(symbol, level, height_)) {
      count = level_[level]->Rank1(pos) - before;
      start = occ_->GetField(masked);
      pos = count + start - 1;
    } else {
      count = pos - start + before - level_[level]->Rank1(pos) + 1;
      masked += ((cds_word)1 << (height_ - level - 1)); 
      pos = count + start - 1;
    }

    if (count == 0) return 0;
  }
  return count;
}

cds_word WaveletTreeNoPtrs::Count(cds_word symbol) const {
  cds_word mapped = am_->Map(symbol);
  return occ_->GetField(mapped + 1) - occ_->GetField(mapped) + 1;
}

cds_word WaveletTreeNoPtrs::Select(cds_word symbol, cds_word pos) const {
  symbol = am_->Map(symbol);
    
  cds_word mask = ((cds_word)1 << height_) - 2;
  cds_word sum = 2;

  for (cds_word level = height_ - 1; level+1 >= 1; level--) {
    
    cds_word start = GetStart(symbol, mask);
    start = occ_->GetField(start);

    cds_word ones_start = 0;
    if (start > 0)
      ones_start = level_[level]->Rank1(start - 1);

    if (IsSet(symbol,level, height_)) {
      pos = level_[level]->Select1(ones_start + pos) - start + 1;
    } else {
      pos = level_[level]->Select0(start - ones_start + pos) - start + 1;
    }

    mask <<= 1;
    sum <<= 1;
  }

  return pos - 1;
}

cds_word WaveletTreeNoPtrs::Access(cds_word pos) const {
  cds_word ret = 0;
  cds_word start = 0;
  for (cds_word level = 0; level < height_; level++) {
    cds_word optR, before = 0;
    if (start > 0)
      before = level_[level]->Rank1(start - 1);
    
    if (level_[level]->Access(pos, &optR)) {
      ret |= ((cds_word)1 << (height_ - level - 1));
      pos = optR - 1 - before;
      start = occ_->GetField(ret);
      pos += start;
    } else {
      pos = optR - 1 + before;
    }
  }

  return am_->Unmap(ret);
}

// cds_word WaveletTreeNoPtrs::Access(cds_word pos, cds_word *rank) const {
//   return 0;
// }

cds_word WaveletTreeNoPtrs::GetSize() const {
  cds_word size = 0;
  for (cds_word i = 0; i < height_; i++) {
    size += level_[i]->GetSize();
  }
  size += occ_->GetSize();
  return size;
}

cds_word WaveletTreeNoPtrs::GetSigma() const {
  return max_v_;
}
cds_word WaveletTreeNoPtrs::GetLength() const {
  return n_;
}

void WaveletTreeNoPtrs::Save(ostream &fp) const {
  SaveValue(fp, kWTNoPtrsHdr);
  SaveValue(fp, n_);
  SaveValue(fp, max_v_);
  SaveValue(fp, height_);
  am_->Save(fp);
  occ_->Save(fp);
  for (cds_word i = 0; i < height_; i++)
    level_[i]->Save(fp);
}

WaveletTreeNoPtrs *WaveletTreeNoPtrs::Load(istream &fp) {
  cds_word id;
  id = LoadValue<cds_word>(fp);
  if (id != kWTNoPtrsHdr) {
    return NULL;
  }
  WaveletTreeNoPtrs *ret = new WaveletTreeNoPtrs();
  ret->n_ = LoadValue<cds_word>(fp);
  ret->max_v_ = LoadValue<cds_word>(fp);
  ret->height_ = LoadValue<cds_word>(fp);
  ret->am_ = Mapper::Load(fp);
  ret->am_->Use();
  ret->occ_ = Array::Load(fp);
  ret->occ_->Use();
  ret->level_ = new BitSequence*[ret->height_];
  for (cds_word i = 0; i < ret->height_; i++) {
    ret->level_[i] = BitSequence::Load(fp);
    ret->level_[i]->Use();
  }
  return ret;
}
};
};
