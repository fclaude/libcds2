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

#ifndef SRC_IMMUTABLE_WAVELETTREENOPTRS_H_
#define SRC_IMMUTABLE_WAVELETTREENOPTRS_H_

#include <libcds/io.h>
#include <libcds/libcds.h>
#include <libcds/array.h>
#include <libcds/immutable/mapper.h>
#include <libcds/immutable/sequence.h>
#include <libcds/immutable/bitsequence.h>

namespace cds {
namespace immutable {

const cds_word kWTNoPtrs = 3;

class WaveletTreeNoPtrs : public Sequence {
  public:
    WaveletTreeNoPtrs(const cds::basic::Array *a,  BitSequenceBuilder *bmb,  Mapper *am);
    virtual ~WaveletTreeNoPtrs();
    virtual cds_word Access(cds_word pos) const;
    // virtual cds_word Access(cds_word pos, cds_word *rank) const;
    virtual cds_word Rank(cds_word symbol, cds_word pos) const;
    virtual cds_word Select(cds_word symbol, cds_word j) const;
    virtual cds_word Count(cds_word symbol) const;
    virtual cds_word GetSigma() const;
    virtual cds_word GetLength() const;
    virtual cds_word GetSize() const;
    virtual void Save(ostream &fp) const;
    static WaveletTreeNoPtrs *Load(istream &fp);

  protected:

    BitSequence **level_;
    cds_word height_;
    Mapper *am_;
    cds_word n_;
    cds_word max_v_;
    Array *occ_;

    void BuildLevels(Array *new_array, Array **bitmaps, cds_word ini, cds_word fin, cds_word level);
};
};
};

#endif  // SRC_IMMUTABLE_WAVELETTREENOPTRS_H_
