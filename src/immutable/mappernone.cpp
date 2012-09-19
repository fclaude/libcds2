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

#include <libcds/immutable/mappernone.h>
#include <libcds/io.h>

namespace cds {
namespace immutable {

MapperNone::MapperNone() {}

cds_word MapperNone::Map(cds_word s) const {
  return s;
}

cds_word MapperNone::Unmap(cds_word s) const {
  return s;
}

cds_word MapperNone::GetSize() const {
  return sizeof(MapperNone);
}

void MapperNone::Save(ostream &out) const {
  cds_word wr = MAPPER_NONE_HDR;
  cds::basic::SaveValue(out, wr);
}

MapperNone *MapperNone::Load(istream &input) {
  cds_word rd;
  rd = cds::basic::LoadValue<cds_word>(input);
  if (rd != MAPPER_NONE_HDR) {
    return NULL;
  }
  return new MapperNone();
}
};
};
