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


#include <libcds/immutable/permutation.h>
#include <libcds/immutable/bitsequenceonelevelrank.h>
#include <libcds/cdsexception.h>
#include <libcds/io.h>

#include <algorithm>
#include <vector>

namespace cds {
namespace immutable {

using cds::basic::CDSException;
using cds::basic::LoadValue;
using cds::basic::SaveValue;
using cds::basic::cds_word;

using std::vector;

PermutationMRRR::PermutationMRRR(Array *perm, cds_word rate) {
	this->permutation_ = perm;
	Array *visited = Array::Create(perm->GetLength(), 1);
	visited->Use();

	Array *sampled = Array::Create(perm->GetLength(), 1);
	sampled->Use();
	vector<cds_word> sampled_values;
	for (cds_word i = 0; i < perm->GetLength(); i++) {
		if (visited->GetField(i)) continue;
	
		bool added = false;
		cds_word last = i;
		visited->SetField(i, 1);
		cds_word pos = perm->GetField(i);
		cds_word count = 1;
		while (pos != i) {
			visited->SetField(pos, 1);
			if (count == rate) {
				added = true;
				sampled_values.push_back(last);
				sampled->SetField(pos, 1);
				last = pos;
				count = 0;
			}
			pos = perm->GetField(pos);
			count++;
		}
		if (added) {
			sampled->SetField(pos, 1);
			sampled_values.push_back(last);
		}
	}
	sampled_ = new BitSequenceOneLevelRank(sampled, 20);
	sampled_->Use();
	rev_links_ = Array::Create(sampled_values);
	rev_links_->Use();

	visited->Unuse();
	sampled->Unuse();
}

PermutationMRRR::~PermutationMRRR() {
	rev_links_->Unuse();
	sampled_->Unuse();
	permutation_->Unuse();
}

cds_word PermutationMRRR::Access(cds_word i) const {
	return permutation_->GetField(i);
}

cds_word PermutationMRRR::Reverse(cds_word i) const {
	cds_word pos = i;
	cds_word next1 = permutation_->GetField(pos);
	while (next1 != i) {
		if (sampled_->Access(pos)) {
			pos = rev_links_->GetField(sampled_->Rank1(pos) - 1);
			cds_word next = permutation_->GetField(pos);
			while (next != i) {
				pos = next;
				next = permutation_->GetField(next);
			}
			return pos;
		}
		pos = next1;
		next1 = permutation_->GetField(pos);
	}
	return pos;
}

cds_word PermutationMRRR::GetSize() const {
	return permutation_->GetSize() + sampled_->GetSize() + rev_links_->GetSize();
}

cds_word PermutationMRRR::GetLength() const {
	return permutation_->GetLength();
}

void PermutationMRRR::Save(ostream &out) const {
	return;
}

PermutationMRRR *PermutationMRRR::Load(istream &fp) {
	return NULL;
}
};
};
