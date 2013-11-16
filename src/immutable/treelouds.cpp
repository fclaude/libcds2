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


#include <libcds2/immutable/tree.h>
#include <libcds2/immutable/treelouds.h>
#include <libcds2/cdsexception.h>
#include <libcds2/io.h>

#include <algorithm>
#include <vector>

namespace cds {
namespace immutable {

using cds::basic::CDSException;
using cds::basic::LoadValue;
using cds::basic::SaveValue;
using cds::basic::cds_word;

TreeLouds::TreeLouds(BitSequence *bitmap) {
	bitmap_ = bitmap;
	bitmap_->Use();
}

TreeLouds::~TreeLouds() {
	if (bitmap_ != NULL) {
		bitmap_->Unuse();
		bitmap_ = NULL;
	}
}

cds_word TreeLouds::Parent(cds_word i) const {
	cds_word sel = bitmap_->Rank0(i) - 1;
	if (sel == 0) {
		return (cds_word)-1;
	}
	cds_word range = bitmap_->Select1(sel);
	return bitmap_->SelectNext0(range);
}

cds_word TreeLouds::Child(cds_word i, cds_word j) const {
	cds_word prev = bitmap_->SelectPrev0(i-1);
	cds_word fst = prev + j + 1;
	cds_word sel = bitmap_->Rank1(fst);
	return bitmap_->Select0(sel + 1);
}

cds_word TreeLouds::Degree(cds_word i) const {
	cds_word prev = bitmap_->SelectPrev0(i-1);
	return i - prev - 1;
}

cds_word TreeLouds::NextSibling(cds_word i) const {
	cds_word sel = bitmap_->Rank0(i) - 1;
	if (sel == 0) {
		return (cds_word)-1;
	}
	cds_word range = bitmap_->Select1(sel);
	if (bitmap_->Access(range+1) == 0)
		return (cds_word)-1;
	return bitmap_->Select0(sel + 2);
}

cds_word TreeLouds::PrevSibling(cds_word i) const {
	cds_word sel = bitmap_->Rank0(i) - 1;
	if (sel == 0) {
		return (cds_word)-1;
	}
	cds_word range = bitmap_->Select1(sel);
	if (range == 0 || bitmap_->Access(range - 1) == 0)
		return (cds_word)-1;
	return bitmap_->Select0(sel);
}

cds_word TreeLouds::GetNodes() const {
	return bitmap_->CountZeros();
}

cds_word TreeLouds::GetSize() const {
	return bitmap_->GetSize();
}

void TreeLouds::Save(ostream &fp) const {
	SaveValue(fp, kTreeLoudsHdr);
	bitmap_->Save(fp);
}

TreeLouds *TreeLouds::Load(istream &fp) {
  cds_word rd = cds::basic::LoadValue<cds_word>(fp);
  if (rd != kTreeLoudsHdr) {
	  cds_word pos = fp.tellg();
	  fp.seekg(pos - sizeof(cds_word));
	  return NULL;
  }
  TreeLouds *ret = new TreeLouds();
  ret->bitmap_ = BitSequence::Load(fp);
  if (ret->bitmap_ == NULL) {
  	cds_word pos = fp.tellg();
	fp.seekg(pos - sizeof(cds_word));
	delete ret;
	return NULL;
  }
  ret->bitmap_->Use();
  return ret;
}
};
};