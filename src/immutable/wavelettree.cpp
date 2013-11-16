/********************************************************************************
Copyright (c) 2012, Francisco Claude,Roberto Konow
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
#include <libcds2/immutable/wavelettree.h>


namespace cds {
namespace immutable {
WaveletTree::WaveletTree(const cds::basic::Array *a,  Coder *coder,  BitSequence *bmb,  Mapper *am) {
  bmb->Use();
  am->Use();
  coder->Use();
  n_ = a->GetLength();
  am_ = am;
  max_v_ = a->GetMax();
  coder_ = coder;
  root_ = new WtNodeInternal(a, 0, coder, bmb);
  root_->Use();
  bmb->Unuse();
}

WaveletTree::~WaveletTree() {
  this->root_->Unuse();
  this->am_->Unuse();
  this->coder_->Unuse();
}
cds_word WaveletTree::Rank(cds_word symbol, cds_word pos) const {
  return this->root_->Rank(this->am_->Map(symbol), pos, 0, this->coder_);
}

cds_word WaveletTree::Count(cds_word s) const {
  return root_->Rank(this->am_->Map(s), this->n_ - 1, 0, this->coder_);
}

cds_word WaveletTree::Select(cds_word symbol, cds_word pos) const {
  cds_word ret = root_->Select(am_->Map(symbol), pos, 0, coder_);
  if (ret == ((cds_word) - 1)) {
    return (cds_word) - 1;
  }
  return ret - 1;
}

cds_word WaveletTree::Access(cds_word pos) const {
  return am_->Unmap(this->root_->Access(pos));
}

cds_word WaveletTree::Access(cds_word pos, cds_word *rank) const {
  return root_->Access(pos, rank);
}

cds_word WaveletTree::GetSize() const {
  return sizeof(WaveletTree) + sizeof(cds_word) + root_->GetSize() + am_->GetSize() + coder_->GetSize();
}
cds_word WaveletTree::GetSigma() const {
  return max_v_;
}
cds_word WaveletTree::GetLength() const {
  return n_;
}

void WaveletTree::Save(ostream &fp) const {
  cds_word wr = kWTPtrsHdr;
  cds::basic::SaveValue(fp, wr);
  cds::basic::SaveValue(fp, n_);
  coder_->Save(fp);
  am_->Save(fp);
  // root_->Save(fp);
}

WaveletTree *WaveletTree::Load(istream &fp) {
  return NULL;
}
//   cds_word rd = cds::basic::LoadValue<cds_word>(fp);
//   if (rd != kWTPtrsHdr) {
//     return NULL;
//   }
//   WaveletTree *ret = new WaveletTree();
//   ret->n_ = cds::basic::LoadValue<cds_word>(fp);
//   ret->coder_ = Coder::Load(fp);
//   ret->coder_->Use();
//   assert(ret->coder_ != NULL);
//   ret->am_ = Mapper::Load(fp);
//   ret->am_->Use();
//   assert(ret->am_ != NULL);
//   // ret->root_ = WtNode::Load(fp);
//   assert(ret->root_ != NULL);
//   return ret;
// }


WaveletTree::WtNode *WaveletTree::WtNode::Load(istream &fp) {
  return NULL;
}

WaveletTree::WtNodeInternal *WaveletTree::WtNodeInternal::Load(istream &fp) {
  return NULL;
}

WaveletTree::WtNodeLeaf *WaveletTree::WtNodeLeaf::Load(istream &fp) {
  return NULL;
}

WaveletTree::WtNodeInternal::WtNodeInternal(const cds::basic::Array *seq, cds_word l,  Coder *c, BitSequence *bmb) {

}

WaveletTree::WtNodeLeaf::WtNodeLeaf(cds_word symbol, cds_word count) {

}
//   cds_word rd = cds::basic::LoadValue<cds_word>(fp);
//   if (rd == kWTNodeNullHdr) {
//     return NULL;
//   }
//   cds_word pos = fp.tellg();
//   fp.seekg(pos - sizeof(cds_word));
//   switch (rd) {
//     case kWTNodeInternalHdr:
//       return WtNodeInternal::Load(fp);
//     case kWTNodeLeafHdr:
//       return WtNodeLeaf::Load(fp);
//   }
//   return NULL;
// }
};
};
