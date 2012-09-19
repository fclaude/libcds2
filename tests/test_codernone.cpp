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

#include <gtest/gtest.h>
#include <libcds/immutable/codernone.h>

using cds::immutable::CoderNone;
using cds::basic::cds_word;

uint seed_test_array = 20;

void testCoder(cds_word n) {
  CoderNone *cod = new CoderNone();
  cod->Use();
  cds_word *stream = new cds_word[n * 64];
  cds_word pos = 0;
  cds_word *symbols = new cds_word[n];
  for (cds_word i = 0 ; i < n ; i++) {
    symbols[i] = rand_r(&seed_test_array);
    cod->Encode(symbols[i], stream, &pos);
  }

  cds_word pos2 = 0;
  for (cds_word i = 0 ; i < n ; i++) {
    ASSERT_EQ(cod->Decode(stream, &pos2), symbols[i]);
  }
  cod->Unuse();
}

TEST(CoderNone, EncodeRandomElements) {
  for (cds_word i = 10; i <= 10000; i += 10) {
    testCoder(i);
  }
}
