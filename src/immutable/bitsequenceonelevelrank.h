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


#ifndef SRC_IMMUTABLE_BITSEQUENCEONELEVELRANK_H_
#define SRC_IMMUTABLE_BITSEQUENCEONELEVELRANK_H_


#include <libcds/array.h>
#include <libcds/libcds.h>
#include <libcds/io.h>
#include <libcds/immutable/bitsequence.h>

#include <fstream>

namespace cds {
namespace immutable {

using cds::basic::cds_word;
using std::ifstream;
using std::ofstream;

using cds::basic::Array;
using cds::basic::ArrayTpl;
using cds::basic::popcount;

/** Base class for static bitsequences, contains many abstract functions,
 *  so this can't be instantiated. It includes base implementations for
 *  rank0, select0 and select1 based on rank0.
 *
 *  @author Francisco Claude
 */
class BitSequenceOneLevelRank : public BitSequence {
  public:
    BitSequenceOneLevelRank(Array *bitmap, cds_word sampling_rate);

    virtual ~BitSequenceOneLevelRank() {}
    // virtual cds_word Select0(const cds_word i) const;
    virtual cds_word Rank1(const cds_word i) const;
    virtual cds_word Select1(const cds_word i) const;
    virtual bool Access(const cds_word i) const;
    virtual cds_word GetLength() const;
    virtual cds_word GetSize() const;
    virtual void Save(ofstream &fp) const;

    static BitSequenceOneLevelRank *Load(ifstream &fp);

  protected:
    ArrayTpl<1> *bitmap_;
    cds_word sampling_rate_;
    Array *sampling_;
    cds_word length_;
};
};
};

#endif  // SRC_IMMUTABLE_BITSEQUENCEONELEVELRANK_H_
