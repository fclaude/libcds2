/**
 *    bitsequence.cpp
 *    Copyright (C) 2008  Francisco Claude F.
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

#include <libcds/immutable/bitsequence.h>
#include <libcds/cdsexception.h>
#include <libcds/io.h>

namespace cds {
namespace immutable {

using cds::basic::CDSException;
using cds::basic::LoadValue;
using cds::basic::SaveValue;

cds_word BitSequence::Rank0(const cds_word i) const {
  return i + 1 - Rank1(i);
}

cds_word BitSequence::Rank1(const cds_word i) const {
  if (i >= length_) {
    throw CDSException("Out of bounds");
  }
  cds_word ini = 1;
  cds_word fin = CountOnes();
  if (fin == 0) {
    return 0;
  }
  if (fin == length_) {
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

cds_word BitSequence::Select0(const cds_word i) const {
  cds_word ones = CountOnes();
  if (i > length_ - ones) {
    return length_;
    // throw CDSException("Out of bounds");
  }
  if (i == 0) {
    return (cds_word) - 1;
  }
  if (ones == 0) {
    return i - 1;
  }
  cds_word ini = 0;
  cds_word fin = length_ - 1;
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
    return length_;
    // throw CDSException("Out of bounds");
  }
  if (i == 0) {
    return (cds_word) - 1;
  }
  if (ones == length_) {
    return i - 1;
  }
  cds_word ini = 0;
  cds_word fin = length_ - 1;
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
  cds_word v = Rank1(i);
  if (v < 2) {
    return (cds_word) - 1;
  }
  return Select1(v - 1);
}

cds_word BitSequence::SelectNext0(const cds_word i) const {
  return Select0((i == 0 ? 0 : Rank0(i - 1)) + 1);
}

cds_word BitSequence::SelectPrev0(const cds_word i) const {
  size_t v = Rank0(i);
  if (v < 2) {
    return (size_t) - 1;
  }
  return Select0(v - 1);
}

bool BitSequence::Access(const cds_word i) const {
  return (Rank1(i) - (i != 0 ? Rank1(i - 1) : 0)) > 0;
}

cds_word BitSequence::GetLength() const {
  return length_;
}

cds_word BitSequence::CountOnes() const {
  return Rank1(length_ - 1);
}

cds_word BitSequence::CountZeros() const {
  return Rank0(length_ - 1);
}

BitSequence *BitSequence::Load(ifstream &fp) {
  cds_word r = LoadValue<cds_word>(fp);
  size_t pos = fp.tellg();
  fp.seekg(pos - sizeof(cds_word));
  switch (r) {
      // case DARRAY_HDR: return BitSequenceDArray::load(fp);
    default:
      throw CDSException("Unknown type");
  }
  return NULL;
}
};
};
