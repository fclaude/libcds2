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

    * Neither the name of libcds. nor the names of its
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


#include <libcds/libcds.h>
#include <libcds/time.h>
#include <libcds/io.h>
#include <libcds/cdsexception.h>

using cds::basic::Tokenize;
using cds::basic::TransformString;
using cds::basic::SaveValue;
using cds::basic::LoadValue;
using cds::basic::LoadFile;
using cds::basic::CDSException;
using cds::basic::Timer;
using cds::basic::msb;
using cds::basic::lsb;
using cds::basic::cds_word;
using cds::basic::cds_uint;
using cds::basic::cds_uchar;
using cds::basic::kMaxCDSUchar;
using cds::basic::kMaxCDSUshort;
using cds::basic::kMaxCDSUint;
using cds::basic::kMaxCDSWord;
using cds::basic::kMaxCDSUlong;
using cds::basic::BitZero;
using cds::basic::BitGet;
using cds::basic::BitSet;
using cds::basic::kWordSize;
using cds::basic::kDoubleWordSize;
using cds::basic::SetField;
using cds::basic::SetVarField;
using cds::basic::GetField;
using cds::basic::GetVarField;
using cds::basic::GetField8;
using cds::basic::GetField16;
using cds::basic::GetField32;


int main(int argc, char **argv) {
  return 0;
}
