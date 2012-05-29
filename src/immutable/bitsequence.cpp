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


#include <libcds/immutable/bitsequence.h>
#include <libcds/cdsexception.h>
#include <libcds/io.h>

namespace cds {
namespace immutable {

using cds::basic::CDSException;
using cds::basic::LoadValue;
using cds::basic::SaveValue;

cds_word BitSequence::Rank0(const cds_word i) const {
  assert(i < GetLength());
  return i + 1 - Rank1(i);
}

cds_word BitSequence::Rank1(const cds_word i) const {
  assert(i < GetLength());
  if (i >= GetLength()) {
    throw CDSException("Out of bounds");
  }
  cds_word ini = 1;
  cds_word fin = CountOnes();
  if (fin == 0) {
    return 0;
  }
  if (fin == GetLength()) {
    return i + 1;
  }
  while (ini < fin) {
    cds_word pos = (ini + fin) / 2;
    cds_word bp = Select1(pos);
    if (bp == i) {
      return pos;
    }
    if (bp < i) {
      ini = pos + 1;
    } else {
      fin = pos - 1;
    }
  }
  if (Select1(ini) > i) {
    return ini - 1;
  }
  return ini;
}

cds_word BitSequence::Rank1(const cds_word i, bool *access) const {
  assert(i < GetLength());
  *access = Access(i);
  return Rank1(i);
}

cds_word BitSequence::Rank0(const cds_word i, bool *access) const {
  assert(i < GetLength());
  *access = Access(i);
  return Rank0(i);
}

cds_word BitSequence::Select0(const cds_word i) const {
  cds_word ones = CountOnes();
  if (i > GetLength() - ones) {
    return GetLength();
    // throw CDSException("Out of bounds");
  }
  if (i == 0) {
    return (cds_word) - 1;
  }
  if (ones == 0) {
    return i - 1;
  }
  cds_word ini = 0;
  cds_word fin = GetLength() - 1;
  while (ini < fin) {
    cds_word pos = (ini + fin) / 2;
    cds_word br = Rank0(pos);
    if (br < i) {
      ini = pos + 1;
    } else {
      fin = pos;
    }
  }
  return ini;
}

cds_word BitSequence::Select1(const cds_word i) const {
  cds_word ones = CountOnes();
  if (i > ones) {
    return GetLength();
    // throw CDSException("Out of bounds");
  }
  if (i == 0) {
    return (cds_word) - 1;
  }
  if (ones == GetLength()) {
    return i - 1;
  }
  cds_word ini = 0;
  cds_word fin = GetLength() - 1;
  while (ini < fin) {
    cds_word pos = (ini + fin) / 2;
    cds_word br = Rank1(pos);
    if (br < i) {
      ini = pos + 1;
    } else {
      fin = pos;
    }
  }
  return ini;
}

cds_word BitSequence::SelectNext1(const cds_word i) const {
  return Select1((i == 0 ? 0 : Rank1(i - 1)) + 1);
}

cds_word BitSequence::SelectPrev1(const cds_word i) const {
  bool value;
  cds_word v = Rank1(i, &value);
  if (value == true) {
    return i;
  } else {
    if (v == 0) {
      return (cds_word) - 1;
    }
    return Select1(v);
  }
}

cds_word BitSequence::SelectNext0(const cds_word i) const {
  return Select0((i == 0 ? 0 : Rank0(i - 1)) + 1);
}

cds_word BitSequence::SelectPrev0(const cds_word i) const {
  bool value;
  cds_word v = Rank0(i, &value);
  if (value == false) {
    return i;
  } else {
    if (v == 0) {
      return (cds_word) - 1;
    }
    return Select0(v);
  }
}

bool BitSequence::Access(const cds_word i) const {
  return (Rank1(i) - (i != 0 ? Rank1(i - 1) : 0)) > 0;
}

cds_word BitSequence::CountOnes() const {
  return Rank1(GetLength() - 1);
}

cds_word BitSequence::CountZeros() const {
  return Rank0(GetLength() - 1);
}

BitSequence *BitSequence::Load(ifstream &fp) {
  cds_word r = LoadValue<cds_word>(fp);
  size_t pos = fp.tellg();
  fp.seekg(pos - sizeof(cds_word));
  switch (r) {
      // case DARRAY_HDR: return BitSequenceDArray::load(fp);
    default:
      assert(false);
      throw CDSException("Unknown type");
  }
  return NULL;
}
};
};
