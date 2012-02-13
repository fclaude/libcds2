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

    * Neither the name of Zend Technologies USA, Inc. nor the names of its
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


#ifndef TESTS_TEST_IOH_H_
#define TESTS_TEST_IOH_H_

#include <libcds/io.h>
#include <libcds/cdsexception.h>

using cds::basic::Tokenize;
using cds::basic::TransformString;
using cds::basic::SaveValue;
using cds::basic::LoadValue;
using cds::basic::LoadFile;
using cds::basic::CDSException;

#include <string>
#include <vector>
#include <sstream>


using std::vector;
using std::string;
using std::stringbuf;
using std::ios_base;
using std::iostream;

bool cmp_vect(vector<string> v1, vector<string> v2) {
  if (v1.size() != v2.size()) {
    return false;
  }
  for (size_t i = 0; i < v1.size(); i++)
    if (v1[i] != v2[i]) {
      return false;
    }
  return true;
}


TEST(Tokenize, Empty) {
  vector<string> emptyv;
  vector<string> res = Tokenize("", ' ');
  bool expres = true;
  ASSERT_EQ(expres, cmp_vect(emptyv, res));
}


TEST(Tokenize, OneString) {
  vector<string> emptyv;
  emptyv.push_back("testing");
  vector<string> res = Tokenize("testing", ' ');
  bool expres = true;
  ASSERT_EQ(expres, cmp_vect(emptyv, res));
}


TEST(Tokenize, TwoStrings) {
  vector<string> emptyv;
  emptyv.push_back("testing");
  emptyv.push_back("function");
  vector<string> res = Tokenize("testing function", ' ');
  bool expres = true;
  ASSERT_EQ(expres, cmp_vect(emptyv, res));
  emptyv.clear();
  emptyv.push_back("testing");
  emptyv.push_back("function");
  res = Tokenize("testing;function", ';');
  ASSERT_EQ(expres, cmp_vect(emptyv, res));
}


TEST(Tokenize, ThreeStrings) {
  vector<string> emptyv;
  emptyv.push_back("testing");
  emptyv.push_back("this");
  emptyv.push_back("function");
  vector<string> res = Tokenize("testing this function", ' ');
  bool expres = true;
  ASSERT_EQ(expres, cmp_vect(emptyv, res));
  emptyv.clear();
  emptyv.push_back("testing");
  emptyv.push_back("this");
  emptyv.push_back("function");
  res = Tokenize("testing;this;function", ';');
  ASSERT_EQ(expres, cmp_vect(emptyv, res));
}


TEST(Tokenize, TwoStringsBegEnd) {
  vector<string> emptyv;
  emptyv.push_back("testing");
  emptyv.push_back("function");
  vector<string> res = Tokenize("testing function ", ' ');
  bool expres = true;
  ASSERT_EQ(expres, cmp_vect(emptyv, res));
  emptyv.clear();
  emptyv.push_back("testing");
  emptyv.push_back("function");
  res = Tokenize(";testing;function;", ';');
  ASSERT_EQ(expres, cmp_vect(emptyv, res));
}


TEST(Tokenize, ThreeStringsMid) {
  vector<string> emptyv;
  emptyv.push_back("testing");
  emptyv.push_back("this");
  emptyv.push_back("function");
  vector<string> res = Tokenize("testing  this function", ' ');
  bool expres = true;
  ASSERT_EQ(expres, cmp_vect(emptyv, res));
  emptyv.clear();
  emptyv.push_back("testing");
  emptyv.push_back("this");
  emptyv.push_back("function");
  res = Tokenize("testing;;this;function", ';');
  ASSERT_EQ(expres, cmp_vect(emptyv, res));
}


TEST(Transform, All) {
#if __LP64__
  cds_word i1 = (cds_word)12345678901234ull;
  cds_word v1 = TransformString<cds_word>("12345678901234");
  ASSERT_EQ(i1, v1);
#endif
  cds_uint i2 = 123456789u;
  cds_uint v2 = TransformString<cds_uint>("123456789");
  ASSERT_EQ(i2, v2);
  cds_uchar i3 = '1';
  cds_uchar v3 = TransformString<cds_uchar>("12");
  ASSERT_EQ(i3, v3);
}


TEST(SaveLoadValue, SingleValues) {
  const cds_uint N = 10000;
  char buffer[N];
  stringbuf *sbuf = new stringbuf(ios_base::in | ios_base::out);
  sbuf->pubsetbuf(buffer, N);
  iostream io(sbuf);
  cds_word i1 = (cds_word)12345678901234ull;
  cds_uint i2 = 123456789u;
  cds_uchar i3 = '1';
  SaveValue(io, i1);
  SaveValue(io, i2);
  SaveValue(io, i3);
  io.seekg(0, ios_base::beg);
  cds_word v1 = LoadValue<cds_word>(io);
  cds_uint v2 = LoadValue<cds_uint>(io);
  cds_uchar v3 = LoadValue<cds_uchar>(io);
  ASSERT_EQ(i1, v1);
  ASSERT_EQ(i2, v2);
  ASSERT_EQ(i3, v3);
}


TEST(SaveLoadValue, ManyValues) {
  const cds_uint N = 1000000;
  char buffer[N];
  stringbuf *sbuf = new stringbuf(ios_base::in | ios_base::out);
  sbuf->pubsetbuf(buffer, N);
  iostream io(sbuf);
  cds_word arr[N / 10];
  for (cds_word i = 0; i < N / 10; i++) {
    arr[i] = i;
  }
  SaveValue(io, arr, N / 10);
  io.seekg(0, ios_base::beg);
  cds_word *v1 = LoadValue<cds_word>(io, N / 10);
  for (cds_word i = 0; i < N / 10; i++) {
    ASSERT_EQ(arr[i], v1[i]);
  }
  delete [] v1;
}


TEST(LoadFile, IO) {
  string name = "inputs/input1";
  bool exit = true;
  bool failed = false;
#if __LP64__
  try {
    cds_word len;
    cds_word *T = LoadFile<cds_word>(name, len);
    ASSERT_EQ(exit, failed);
    delete [] T;
  } catch (CDSException &e) {
    string expected = "Error, invalid input";
    ASSERT_EQ(expected, e.what());
  }
#endif
  try {
    cds_word len;
    cds_uint *T = LoadFile<cds_uint>(name, len);
    cds_uint V[] = {
      0x73696854, 0x20736920, 0x69206e61,
      0x7475706e, 0x0a212121
    };
    cds_word explen = 5;
    ASSERT_EQ(explen, len);
    for (cds_word i = 0; i < explen; i++) {
      ASSERT_EQ(V[i], T[i]);
    }
    delete [] T;
  } catch (CDSException &e) {
    ASSERT_EQ(exit, failed) << e.what();
  }
  try {
    cds_word len;
    name = "inputs/input0";
    cds_word *T = LoadFile<cds_word>(name, len);
    ASSERT_EQ(exit, failed);
    delete [] T;
  } catch (CDSException &e) {
    string expected = "Error opening file";
    ASSERT_EQ(expected, e.what());
  }
}

#endif  // TESTS_TEST_IOH_H_
