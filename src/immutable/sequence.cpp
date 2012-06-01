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


#include <libcds/immutable/sequence.h>
#include <libcds/cdsexception.h>
#include <libcds/io.h>

#include <algorithm>

namespace cds {
namespace immutable {

using cds::basic::CDSException;
using cds::basic::LoadValue;
using cds::basic::SaveValue;

cds_word Sequence::Access(const cds_word i) const {
  for (cds_word s = 0; s < GetSigma(); s++) {
    if (Rank(s, i - 1) - Rank(s, i) == 1) {
      return s;
    }
  }
  assert(false);
  throw CDSException("Unknown symbol, cannot access");
}

cds_word Sequence::Access(const cds_word i, cds_word *rank) const {
  cds_word s = Access(i);
  *rank = Rank(s, i);
  return s;
}

cds_word Sequence::Rank(const cds_word s, const cds_word i) const {
  cds_word count = 0;
  for (cds_word pos = 0; pos <= i; pos++)
    if (Access(pos) == s) {
      count++;
    }
  return count;
}

cds_word Sequence::Select(const cds_word s, const cds_word j) const {
  if (j == 0) {
    return static_cast<cds_word>(-1);
  }
  cds_word count = 0;
  for (cds_word pos = 0; pos <= GetLength(); pos++) {
    if (Access(pos) == s) {
      count++;
    }
    if (count == j) {
      return pos;
    }
  }
  return GetLength();
}

cds_word Sequence::Count(const cds_word s) const {
  return Rank(s, GetLength() - 1);
}

cds_word Sequence::GetSigma() const {
  cds_word max = 0;
  for (cds_word i = 0; i < GetLength(); i++) {
    max = std::max(Access(i), max);
  }
  return max;
}
};
};
