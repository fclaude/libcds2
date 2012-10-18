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
#include <libcds/immutable/mapper.h>
#include <libcds/immutable/mappernone.h>
#include <libcds/immutable/coder.h>
#include <libcds/immutable/codernone.h>
#include <libcds/immutable/bitsequence.h>
#include <libcds/immutable/bitsequenceonelevelrank.h>
#include <libcds/libcds.h>
#include <libcds/array.h>
#include <iostream>

using std;
using cds::basic;
using cds::immutable;

int main() {
  // cds::basic::PrintBinary(35);
  cds_word N = 10000000;
  Array *a = Array::Create(N, cds::basic::msb(N + 1));
  a->Use();

  for (cds_word i = 0; i < N; i++) {
    a->SetField(i, i + 1);
  }
  for (cds_word i = 0; i < N; i++) {
    if (a->GetField(i) != i + 1) {
      cout << "Error, values do not match" << endl;
    }
  }
  cout << "Size for a: " << a->GetSize();
  a->Unuse();

  // Array *a2 = Array::Create(N, 1);
  // a2->Use();
  // for (cds_word i = 0; i < N; i++) {
  //  if (i%2) {
  //    a2->SetField(i, 1);
  //  }
  // }
  // cout << "Bitmap of length " << N << " requires " << a2->GetSize() << " bytes" << endl;
  // BitSequenceOneLevelRank *b = new BitSequenceOneLevelRank(a2, 20);
  // for (cds_word i = 0; i < N; i++) {
  //  if (b->Access(i) != (a2->GetField(i)>0)) {
  //    cout << "Error, values do not match (" << b->Access(i) << ", " << a2->GetField(i) << ")" << endl;
  //  }
  // }
  // a2->Unuse();
  // b->Unuse();
  // cout << "We are done!" << endl;
}
