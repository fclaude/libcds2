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

#include <libcds2/io.h>
#include <libcds2/libcds.h>
#include <libcds2/array.h>
#include <libcds2/immutable/coder.h>
#include <libcds2/immutable/mapper.h>
#include <libcds2/immutable/sequence.h>
#include <libcds2/immutable/bitsequence.h>

namespace cds {
namespace immutable {
const cds_word kWTNodeNullHdr = 0;
const cds_word  kWTNodeInternalHdr = 2;
const cds_word  kWTNodeLeafHdr = 3;
class WaveletTree : public Sequence {
  public:
    WaveletTree(const cds::basic::Array *a,  Coder *coder,  BitSequence *bmb,  Mapper *am);
    virtual ~WaveletTree();
    virtual cds_word Access(cds_word pos) const;
    virtual cds_word Access(cds_word pos, cds_word *rank) const;
    virtual cds_word Rank(cds_word symbol, cds_word pos) const;
    virtual cds_word Select(cds_word symbol, cds_word j) const;
    virtual cds_word Count(const cds_word s) const;
    virtual cds_word GetSigma() const;
    virtual cds_word GetLength() const;
    virtual cds_word GetSize() const;
    virtual void Save(ostream &fp) const;
    static WaveletTree *Load(istream &fp);

  protected:
    WaveletTree();


    class WtNode : public cds::basic::ReferenceCounted {
      public:
        virtual ~WtNode() {}
        virtual cds_word Rank(cds_word symbol, cds_word pos, cds_word l, Coder *c) const = 0;
        virtual cds_word Select(cds_word symbol, cds_word pos, cds_word l, Coder *c) const = 0;
        virtual cds_word Access(cds_word pos) const = 0;
        virtual cds_word Access(cds_word pos, cds_word *rankp) const = 0;
        virtual cds_word GetSize() const = 0;
        virtual void Save(ostream &fp) const = 0;
        static WtNode *Load(istream &fp);
    };

    class WtNodeInternal: public WtNode {
      public:
        WtNodeInternal(const cds::basic::Array *seq, cds_word l,  Coder *c, BitSequence *bmb);
        virtual ~WtNodeInternal();
        virtual cds_word Rank(cds_word symbol, cds_word pos, cds_word level, Coder *c) const = 0;
        virtual cds_word Select(cds_word symbol, cds_word pos, cds_word level, Coder *c) const = 0;
        virtual cds_word Access(cds_word pos) const = 0;
        virtual cds_word Access(cds_word pos, cds_word *rankp) const = 0;
        virtual cds_word GetSize() const = 0;
        virtual void Save(ostream &fp) const = 0;
        static WtNodeInternal *Load(istream &fp);

      protected:
        WtNodeInternal();
        WtNode *leftchild_;
        WtNode *right_child_;
        BitSequence *bitmap_;
    };

    class WtNodeLeaf: public WtNode {
      public:
        WtNodeLeaf(cds_word symbol, cds_word count);
        virtual ~WtNodeLeaf();
        virtual cds_word rank(cds_word symbol, cds_word pos, cds_word l, Coder *c) const = 0;
        virtual cds_word select(cds_word symbol, cds_word pos, cds_word l, Coder *c) const = 0;
        virtual cds_word access(cds_word pos) const = 0;
        virtual cds_word access(cds_word pos, cds_word *rank) const = 0;
        virtual cds_word getSize() const = 0;
        virtual void Save(ostream &fp) const = 0;
        static WtNodeLeaf *Load(istream &fp);

      protected:
        WtNodeLeaf();
        cds_word symbol_;
        cds_word count_;
    };

    WtNode *root_;
    Coder *coder_;
    Mapper *am_;
    cds_word n_;
    cds_word max_v_;
    bool test_;
};
};
};

#endif  // SRC_IMMUTABLE_WAVELETTREE_H_
