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

#include <libcds/static/bitsequence.h>

namespace cds_static
{

  using namespace cds_basic;

  cds_word BitSequence::rank0(const cds_word i) const {
    return i+1-rank1(i);
  }

  cds_word BitSequence::rank1(const cds_word i) const {
    if(i>=length) {
      throw CDSException("Out of bounds");
    }
    if(ones==0) {
      return 0;
    }
    if(ones==length) {
      return i+1;
    }
    cds_word ini = 1;
    cds_word fin = ones;
    while(ini<fin) {
      cds_word pos = (ini+fin)/2;
      cds_word bp = select1(pos);
      if(bp==i) return pos;
      if(bp<i)
	ini = pos+1;
      else
	fin = pos-1;
    }
    if(select1(ini)>i) return ini-1;
    return ini;
  }

  cds_word BitSequence::select0(const cds_word i) const {
      if(i>length-ones) {
	return length;
	//throw CDSException("Out of bounds");
      }
      if(i==0) {
	return (cds_word)-1;
      }
      if(ones==0) {
	return i-1;
      }
      cds_word ini = 0;
      cds_word fin = length-1;
      while(ini<fin) {
	cds_word pos = (ini+fin)/2;
	cds_word br = rank0(pos);
	if(br<i)
	  ini = pos+1;
	else
	  fin = pos;
      }
      return ini;
  }

  cds_word BitSequence::select1(const cds_word i) const
  {
    if(i>ones) {
      return length;
      //throw CDSException("Out of bounds");
    }
    if(i==0) {
      return (cds_word)-1;
    }
    if(ones==length) {
      return i-1;
    }
    cds_word ini = 0;
    cds_word fin = length-1;
    while(ini<fin) {
      cds_word pos = (ini+fin)/2;
      cds_word br = rank1(pos);
      if(br<i)
	ini = pos+1;
      else
	fin = pos;
    }
    return ini;
  }

  cds_word BitSequence::selectNext1(const cds_word i) const {
    return select1((i==0?0:rank1(i-1))+1);
  }

  cds_word BitSequence::selectPrev1(const cds_word i) const {
    cds_word v = rank1(i);
    if(v<2) return (cds_word)-1;
    return select1(v-1);
  }

  cds_word BitSequence::selectNext0(const cds_word i) const {
    return select0((i==0?0:rank0(i-1))+1);
  }

  cds_word BitSequence::selectPrev0(const cds_word i) const {
    size_t v = rank0(i);
    if(v<2) return (size_t)-1;
    return select0(v-1);
  }

  bool BitSequence::access(const cds_word i) const {
    return (rank1(i)-(i!=0?rank1(i-1):0))>0;
  }

  cds_word BitSequence::getLength() const {
    return length;
  }

  csd_word BitSequence::countOnes() const {
    return ones;
  }

  cds_word BitSequence::countZeros() const {
    return length-ones;
  }

  BitSequence * BitSequence::load(ifstream & fp) {
    cds_word r = loadValue<cds_word>(fp);
    size_t pos = fp.tellg();
    fp.seekg(pos-sizeof(cds_word));
    switch(r) {
      //case DARRAY_HDR: return BitSequenceDArray::load(fp);
    default: throw CDSException("Unknown type");
    }
    return NULL;
  }
};
