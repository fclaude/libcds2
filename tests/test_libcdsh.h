/**
 *    test_libcdsh.h
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


/** Test values are what we expect */
TEST(TypeSize,ExpectedValues) {
  unsigned long long v;
  v = (((unsigned long long)1)<<8);
  ASSERT_EQ(v,MAX_CDS_UCHAR);
  v = (((unsigned long long)1)<<8*2);
  ASSERT_EQ(v,MAX_CDS_USHORT);
  v = (((unsigned long long)1)<<8*4);
  ASSERT_EQ(v,MAX_CDS_UINT);
  v = (((unsigned long long)1)<<(8*8-6));
  ASSERT_EQ(v,MAX_CDS_WORD);
  ASSERT_EQ(v,MAX_CDS_ULONG);
  v = 64;
  ASSERT_EQ(v,W);
  v *= 2;
  ASSERT_EQ(v,WW);
}

/** Test popcount */
cds_word seq_popcount(const cds_word x) {
  cds_word v = x;
  cds_word ret = 0;
  while(v!=0) {
    if(v&1) ret++;
    v >>= 1;
  }
  return ret;
}

TEST(Bits,PopcountWord) {
  for(cds_word i=0; i<128*MAX_CDS_UCHAR;i++) {
    ASSERT_EQ(seq_popcount(i),popcount(i));
  }
  for(cds_word i=0; i<128*MAX_CDS_UCHAR;i++) {
    ASSERT_EQ(seq_popcount((i<<32) | i),popcount((i<<32) | i));
  }
}

TEST(Bits,PopcountUint) {
  for(cds_word i=0; i<128*MAX_CDS_UCHAR;i++) {
    ASSERT_EQ(seq_popcount(i),popcount((cds_uint)i));
  }
}

/** Test bitset, bitget and bitzero */
TEST(Bits,BitOperations) {
  cds_word len = 10000;
  cds_word *bm = new cds_word[len/W+1];
  cds_word *pos = new cds_word[len/2];
  bool t=true, f=false;
  for(cds_word i=0;i<len/2;i++) {
    pos[i] = ((cds_word)rand())%len;
    bitzero(bm,2*i);
    ASSERT_EQ(f,bitget(bm,2*i));
    bitset(bm,2*i+1,true);
    ASSERT_EQ(t,bitget(bm,2*i+1));
    bitset(bm,2*i+1,false);
    ASSERT_EQ(f,bitget(bm,2*i+1));
  }
  sort(pos,pos+len/2);
  for(cds_word i=0;i<len/2;i++) {
    bitset(bm,pos[i],true);
    ASSERT_EQ(t,bitget(bm,pos[i]));
  }
  cds_word posn = 0;
  for(cds_word i=0;i<len/2;i++) {
    while(posn<pos[i]) {
      ASSERT_EQ(f,bitget(bm,posn));
      posn++;
    }
    if(posn==pos[i]) {
      ASSERT_EQ(t,bitget(bm,posn));
      bitset(bm,posn,false);
    }
    posn = pos[i]+1;
  }
  for(cds_word i=0;i<len;i++)
    ASSERT_EQ(f,bitget(bm,i));
  delete [] bm;
  delete [] pos;
}

/** Test bits function */

/** Test msb() */
cds_word my_msb(cds_word x) {
  cds_word ret = 0;
  while(x) {
    x >>= 1;
    ret++;
  }
  return ret;
}

TEST(Bits,Msb) {
  for(cds_word i=0; i<MAX_CDS_UINT/1000;i++) {
    cds_word val = rand()%MAX_CDS_WORD;
    ASSERT_EQ(my_msb(val),msb(val));
  }
}

/** Test lsb() */
cds_word my_lsb(cds_word x) {
  cds_word ret = 0;
  if(x==0) return sizeof(cds_word)*8;
  while((x&1) == 0) {
    x >>= 1;
    ret++;
  }
  return ret;
}

TEST(Bits,Lsb) {
  for(cds_word c=1;c<MAX_CDS_UINT/1000;c++) {
    cds_word val = rand()%MAX_CDS_WORD;
    ASSERT_EQ(my_lsb(val),lsb(val));
  }
}


/** Test get and set fields */

TEST(Fields,ZeroBits) {
  const cds_word N = 10;
  cds_word exp = 0;
  cds_word vals[N];
  cds_word exp2 = (cds_word)0xFFFFFFFFFFFFFFFFll;
  cds_word exp3 = 1;
  for(cds_word i=0;i<N;i++)
    vals[i] = exp2;

  for(cds_word i=0;i<MAX_CDS_UCHAR;i++) {
    set_field(vals,0,i,rand());
    for(cds_word j=0;j<N;j++)
      ASSERT_EQ(exp2,vals[j]);

    for(cds_word j=0;j<N;j++)
      ASSERT_EQ(exp2,vals[j]);

    ASSERT_EQ(exp,get_field(vals,0,i));
    ASSERT_EQ(exp3,get_var_field(vals,i,i));
  }
  for(cds_word i=0;i<N;i++)
    ASSERT_EQ(exp2,vals[i]);
}

void test_fields_k(const cds_word k) {
  const cds_word N = 1000000;
  const cds_word array_length = N*k/W+1;
  cds_word mask = ((cds_word)1<<k)-1;
  cds_word A[array_length];
  for(cds_word i=0;i<array_length;i++)
    A[i] = 0;

  for(cds_word i=0; i<N; i++) {
    set_field(A,k,i,i&mask);
    cds_word exp = i&mask;
    ASSERT_EQ(exp,get_field(A,k,i));
  }

  for(cds_word i=0; i<N; i++) {
    set_var_field(A,k*i,k*(i+1)-1,i&mask);
    cds_word exp = i&mask;
    ASSERT_EQ(exp,get_var_field(A,k*i,k*(i+1)-1));
  }
}

TEST(Fields,AllBits) {
  for(uint i=1;i<=58;i++)
    test_fields_k(i);
}

TEST(Fields,4Bits) {
  const cds_word k = 4;
  const cds_word N = 1000000;
  const cds_word array_length = N*k/W+1;
  cds_word mask = ((cds_word)1<<k)-1;
  cds_word A[array_length];
  for(cds_word i=0;i<array_length;i++)
    A[i] = 0;

  for(cds_word i=0; i<N; i++) {
    cds_word exp = i&mask;
    set_field(A,k,i,exp);
    ASSERT_EQ(exp,get_field(A,k,i));
  }

  for(cds_word i=0; i<N; i++) {
    cds_word exp = i&mask;
    set_var_field(A,k*i,k*(i+1)-1,exp);
    ASSERT_EQ(exp,get_var_field(A,k*i,k*(i+1)-1));
  }
}

TEST(Fields,8Bits) {
  const cds_word k = 8;
  const cds_word N = 1000000;
  const cds_word array_length = N*k/W+1;
  cds_word mask = ((cds_word)1<<k)-1;
  cds_word A[array_length];
  for(cds_word i=0;i<array_length;i++)
    A[i] = 0;

  for(cds_word i=0; i<N; i++) {
    set_field(A,k,i,i&mask);
    cds_word exp = i&mask;
    ASSERT_EQ(exp,get_field8(A,i));
  }

  for(cds_word i=0; i<N; i++) {
    set_var_field(A,k*i,k*(i+1)-1,i&mask);
    cds_word exp = i&mask;
    ASSERT_EQ(exp,get_var_field(A,k*i,k*(i+1)-1));
  }
}

TEST(Fields,16Bits) {
  const cds_word k = 16;
  const cds_word N = 1000000;
  const cds_word array_length = N*k/W+1;
  cds_word mask = ((cds_word)1<<k)-1;
  cds_word A[array_length];
  for(cds_word i=0;i<array_length;i++)
    A[i] = 0;

  for(cds_word i=0; i<N; i++) {
    set_field(A,k,i,i&mask);
    cds_word exp = i&mask;
    ASSERT_EQ(exp,get_field16(A,i));
  }

  for(cds_word i=0; i<N; i++) {
    set_var_field(A,k*i,k*(i+1)-1,i&mask);
    cds_word exp = i&mask;
    ASSERT_EQ(exp,get_var_field(A,k*i,k*(i+1)-1));
  }
}

TEST(Fields,32Bits) {
  const cds_word k = 32;
  const cds_word N = 1000000;
  const cds_word array_length = N*k/W+1;
  cds_word mask = ((cds_word)1<<k)-1;
  cds_word A[array_length];
  for(cds_word i=0;i<array_length;i++)
    A[i] = 0;

  for(cds_word i=0; i<N; i++) {
    set_field(A,k,i,i&mask);
    cds_word exp = i&mask;
    ASSERT_EQ(exp,get_field32(A,i));
  }

  for(cds_word i=0; i<N; i++) {
    set_var_field(A,k*i,k*(i+1)-1,i&mask);
    cds_word exp = i&mask;
    ASSERT_EQ(exp,get_var_field(A,k*i,k*(i+1)-1));
  }
}
