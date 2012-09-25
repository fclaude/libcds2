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

#ifndef SRC_IMMUTABLE_WAVELETTREE_H_
#define SRC_IMMUTABLE_WAVELETTREE_H_


#include <libcds/libcds.h>
#include <libcds/io.h>
#include <libcds/immutable/sequence.h>

namespace cds {
namespace immutable {

class WaveletTree : public Sequence {
    public:
        WaveletTree(cds_word *symbols, cds_word n, Coder *coder, BitSequence * bmb, Mapper * am, bool free = false);
        WaveletTree(const Array &a, Coder *coder, BitSequence *bmb, Mapper *am);
        virtual ~WaveletTree();
        virtual uint Access(cds_word pos) const;
        virtual uint Access(cds_word pos, cds_word &rank) const;
        virtual size_t Rank(cds_word symbol, cds_word pos) const;
        virtual size_t Select(cds_word symbol, cds_word j) const;
        virtual size_t Count(const cds_word s) const;
        virtual cds_word GetSigma() const;
        virtual size_t getSize() const;
        virtual void save(ostream &fp) const;
        static WaveletTree * load(istream &fp);
    protected:
        WaveletTree();
        WaveletTreeNode *root_;
        WaveletTreeCoder *coder_;
        Mapper *am_;
        cds_word n_;
        cds_word max_v_;
        bool test_;
};
};
};

#endif  // SRC_IMMUTABLE_WAVELETTREE_H_
