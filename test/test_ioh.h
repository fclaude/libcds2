/**
 *    test_ioh.h
 *    Copyright (C) 2011  Francisco Claude F.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

bool cmp_vect(vector<string> v1, vector<string> v2) {
  if(v1.size()!=v2.size()) return false;
  for(size_t i=0;i<v1.size();i++)
    if(v1[i]!=v2[i]) return false;
  return true;
}

TEST(Tokenize,Empty) {
  vector<string> emptyv;
  vector<string> res = tokenize("",' ');
  bool expres = true;
  ASSERT_EQ(expres,cmp_vect(emptyv,res));
}

TEST(Tokenize,OneString) {
  vector<string> emptyv;
  emptyv.push_back("testing");
  vector<string> res = tokenize("testing",' ');
  bool expres = true;
  ASSERT_EQ(expres,cmp_vect(emptyv,res));
}

TEST(Tokenize,TwoStrings) {
  vector<string> emptyv;
  emptyv.push_back("testing");
  emptyv.push_back("function");
  vector<string> res = tokenize("testing function",' ');
  bool expres = true;
  ASSERT_EQ(expres,cmp_vect(emptyv,res));
  emptyv.clear();
  emptyv.push_back("testing");
  emptyv.push_back("function");
  res = tokenize("testing;function",';');
  ASSERT_EQ(expres,cmp_vect(emptyv,res));
}

TEST(Tokenize,ThreeStrings) {
  vector<string> emptyv;
  emptyv.push_back("testing");
  emptyv.push_back("this");
  emptyv.push_back("function");
  vector<string> res = tokenize("testing this function",' ');
  bool expres = true;
  ASSERT_EQ(expres,cmp_vect(emptyv,res));
  emptyv.clear();
  emptyv.push_back("testing");
  emptyv.push_back("this");
  emptyv.push_back("function");
  res = tokenize("testing;this;function",';');
  ASSERT_EQ(expres,cmp_vect(emptyv,res));
}

TEST(Tokenize,TwoStringsBegEnd) {
  vector<string> emptyv;
  emptyv.push_back("testing");
  emptyv.push_back("function");
  vector<string> res = tokenize("testing function ",' ');
  bool expres = true;
  ASSERT_EQ(expres,cmp_vect(emptyv,res));
  emptyv.clear();
  emptyv.push_back("testing");
  emptyv.push_back("function");
  res = tokenize(";testing;function;",';');
  ASSERT_EQ(expres,cmp_vect(emptyv,res));
}

TEST(Tokenize,ThreeStringsMid) {
  vector<string> emptyv;
  emptyv.push_back("testing");
  emptyv.push_back("this");
  emptyv.push_back("function");
  vector<string> res = tokenize("testing  this function",' ');
  bool expres = true;
  ASSERT_EQ(expres,cmp_vect(emptyv,res));
  emptyv.clear();
  emptyv.push_back("testing");
  emptyv.push_back("this");
  emptyv.push_back("function");
  res = tokenize("testing;;this;function",';');
  ASSERT_EQ(expres,cmp_vect(emptyv,res));
}

TEST(Transform,All) {
  cds_word i1 = (cds_word)12345678901234ull;
  cds_word v1 = transform_string<cds_word>("12345678901234");
  ASSERT_EQ(i1,v1);
  cds_uint i2 = 123456789u;
  cds_uint v2 = transform_string<cds_uint>("123456789");
  ASSERT_EQ(i2,v2);
  cds_uchar i3 = '1';
  cds_uchar v3 = transform_string<cds_uchar>("12");
  ASSERT_EQ(i3,v3);
}

TEST(SaveLoadValue,SingleValues) {
  const cds_uint N = 10000;
  char buffer[N];
  stringbuf * sbuf = new stringbuf(ios_base::in | ios_base::out);
  sbuf->pubsetbuf(buffer,N);
  iostream io(sbuf);
  cds_word i1 = (cds_word)12345678901234ull;
  cds_uint i2 = 123456789u;
  cds_uchar i3 = '1';
  save_value(io,i1);
  save_value(io,i2);
  save_value(io,i3);
  io.seekg(0,ios_base::beg);
  cds_word v1 = load_value<cds_word>(io);
  cds_uint v2 = load_value<cds_uint>(io);
  cds_uchar v3 = load_value<cds_uchar>(io);
  ASSERT_EQ(i1,v1);
  ASSERT_EQ(i2,v2);
  ASSERT_EQ(i3,v3);
}

TEST(SaveLoadValue,ManyValues) {
  const cds_uint N = 1000000;
  char buffer[N];
  stringbuf * sbuf = new stringbuf(ios_base::in | ios_base::out);
  sbuf->pubsetbuf(buffer,N);
  iostream io(sbuf);
  cds_word arr[N/10];
  for(cds_word i=0;i<N/10;i++)
    arr[i] = i;
  save_value(io,arr,N/10);
  io.seekg(0,ios_base::beg);
  cds_word *v1 = load_value<cds_word>(io,N/10);
  for(cds_word i=0;i<N/10;i++)
    ASSERT_EQ(arr[i],v1[i]);
  delete [] v1;
}

TEST(LoadFile,IO) {
  string name = "inputs/input1";
  bool exit = true;
  bool failed = false;
  try {
    cds_word len;
    cds_word * T = load_file<cds_word>(name,len);
    ASSERT_EQ(exit,failed);
    delete [] T;
  } catch(CDSException & e) {
    string expected = "Error, invalid input";
    ASSERT_EQ(expected,e.what());
  }
  try {
    cds_word len;
    cds_uint * T = load_file<cds_uint>(name,len);
    cds_uint V[] = {0x73696854, 0x20736920, 0x69206e61, 
		    0x7475706e, 0x0a212121};
    cds_word explen = 5;
    ASSERT_EQ(explen,len);
    for(cds_word i=0; i<explen; i++)
      ASSERT_EQ(V[i],T[i]);
    delete [] T;
  } catch(CDSException & e) {
    ASSERT_EQ(exit,failed) << e.what();
  }
  try {
    cds_word len;
    name = "inputs/input0";
    cds_word * T = load_file<cds_word>(name,len);
    ASSERT_EQ(exit,failed);
    delete [] T;
  } catch(CDSException & e) {
    string expected = "Error opening file";
    ASSERT_EQ(expected,e.what());
  }
}
