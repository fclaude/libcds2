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

#include <libcds/immutable/codernone.h>
#include <libcds/io.h>

using cds::basic::kWordSize;

namespace cds {
namespace immutable {

CoderNone::CoderNone() { }

void CoderNone::Encode(cds_word symb, cds_word * stream, cds_word *pos) const {
    stream[*pos/kWordSize] = symb;
    *pos+=kWordSize;
}
cds_word CoderNone::Decode(cds_word *stream, cds_word *pos) const {
    cds_word ret = stream[*pos/kWordSize];
    *pos+=kWordSize;
    return ret;
}
cds_word CoderNone::MaxLength() const {
    return kWordSize;
}
cds_word CoderNone::GetSize() const {
    return sizeof(this);
}
void CoderNone::Save(ostream &output) const {
  cds_word wr = CODER_NONE_HDR;
  cds::basic::SaveValue(output, wr);
}
CoderNone *CoderNone::Load(istream &input) {
  cds_word rd;
  rd = cds::basic::LoadValue<cds_word>(input);
  if (rd != CODER_NONE_HDR) {
    return NULL;
  }
  return new CoderNone();
}
};
};
