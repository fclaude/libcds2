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

#ifndef SRC_IMMUTABLE_CODER_H_
#define SRC_IMMUTABLE_CODER_H_

#include <libcds2/libcds.h>

namespace cds {
namespace immutable {

#define CODER_NONE_HDR 4

using cds::basic::cds_word;
using std::ostream;
using std::istream;

class Coder : public cds::basic::ReferenceCounted {
  public:
    Coder();
    virtual ~Coder() { }
    virtual void Encode(cds_word symb, cds_word *stream, cds_word *pos) const = 0;
    virtual cds_word Decode(cds_word *stream, cds_word *pos) const = 0;
    virtual cds_word MaxLength() const = 0;
    virtual cds_word GetSize() const = 0;
    virtual void Save(ostream &output) const = 0;
    static Coder *Load(istream &input);
};
};
};
#endif  // SRC_IMMUTABLE_CODER_H_
