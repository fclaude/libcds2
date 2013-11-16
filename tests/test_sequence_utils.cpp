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

#include <gtest/gtest.h>
#include <libcds2/array.h>
#include <libcds2/immutable/sequence.h>

using cds::immutable::Sequence;
using cds::basic::Array;
using cds::basic::ArrayTpl;
using cds::basic::cds_word;

#include <map>
#include <sstream>

using std::stringbuf;
using std::ios_base;
using std::iostream;
using std::map;

Array *CreateRandomSequence(const cds_word length, const cds_word sigma, unsigned int seed) {
  Array *ret = Array::Create(length, cds::basic::msb(sigma));
  for (cds_word i = 0; i < length; i++) {
    cds_word s = rand_r(&seed) % (sigma + 1);
    ret->SetField(i, s);
  }
  return ret;
}

void TestSequence(Sequence *model, Sequence *tested, bool save_and_load) {
  cds_word length = model->GetLength();
  cds_word sigma = model->GetSigma();

  if (save_and_load) {
    cds_word bufflen = sizeof(cds_word) * (length + sigma) * 2;
    char *buffer = new char[bufflen];
    stringbuf *sbuf = new stringbuf(ios_base::in | ios_base::out);
    sbuf->pubsetbuf(buffer, bufflen);
    iostream io(sbuf);

    tested->Save(io);
    tested->Unuse();
    tested = Sequence::Load(io);
    tested->Use();

    delete sbuf;
    delete []buffer;
  }

  cds_word rec = tested->GetLength();
  ASSERT_EQ(length, rec) << "Error computing length";

  rec = tested->GetSigma();
  ASSERT_EQ(sigma, rec) << "Error retrieving sigma";

  map<cds_word, cds_word> occ;
  for (cds_word i = 0; i < length; i++) {
    cds_word rec = tested->Access(i);
    cds_word exp = model->Access(i);
    if (occ.count(exp) > 0) {
      occ[exp]++;
    } else {
      occ[exp] = 1;
    }
    ASSERT_EQ(exp, rec) << "Error in Access";
    rec = tested->Rank(rec, i);
    exp = model->Rank(exp, i);
    ASSERT_EQ(exp, rec) << "Error in Rank";
    rec = tested->Select(model->Access(i), rec);
    exp = model->Select(model->Access(i), exp);
    ASSERT_EQ(exp, rec) << "Error in Select";
    rec = tested->Rank(sigma - 1, i);
    exp = model->Rank(sigma - 1, i);
    ASSERT_EQ(exp, rec) << "Error in Rank";
  }
  if (save_and_load)
    tested->Unuse();
}
