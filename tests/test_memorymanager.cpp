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
#include <libcds2/dynamic/memorymanager.h>

using cds::dynamic::MemoryManager;
using cds::dynamic::MemoryAddress;
using cds::basic::cds_word;

uint seed_test_array = 20;

void testMemMan1(cds_word n, cds_word len) {
  MemoryManager *m = new MemoryManager(len,  1024);
  MemoryAddress **a = new MemoryAddress*[n];

  for (cds_word i = 0; i < n; i++) {
    a[i] = m->Alloc(len);
    m->SetData(a[i], &i);
    cds_word num = 0;
    m->GetData(a[i], &num);
    ASSERT_EQ(i, num);
    MemoryAddress *ptr = cds::dynamic::GetPtr(a[i]->ptr, a[i]->len, a[i]->offset);
    ASSERT_EQ(a[i], ptr);
  }
  for (cds_word i = 0; i < n; i++) {
    MemoryAddress *ptr = cds::dynamic::GetPtr(a[i]->ptr, a[i]->len, a[i]->offset);
    ASSERT_EQ(a[i], ptr);
  }

  for (cds_word i = 0; i < n; i++) {
    MemoryAddress *ptr = cds::dynamic::GetPtr(a[i]->ptr, a[i]->len, a[i]->offset);
    ASSERT_EQ(a[i], ptr);
    cds_word num = 0;
    m->GetData(a[i], &num);
    ASSERT_EQ(i, num);
    m->Free(a[i]);
    delete a[i];
  }

  delete []a;
  delete m;
}

TEST(MemoryManager, Cycle) {
  for (cds_word i = 10000; i <= 100000; i += 10000) {
    std::cout << "Running i=" << i << std::endl;
    testMemMan1(i, cds::basic::msb(i));
  }
}
