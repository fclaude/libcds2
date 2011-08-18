/**
 *    libcds.h
 *    Copyright (C) 2005, Rodrigo Gonzalez.
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


#pragma once


#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <string>
#include <sstream>
#include <cassert>
#include <vector>
#include <queue>
#include <map>
#include <string>

using namespace std;

#include <sys/types.h>
#include <sys/resource.h>
#include <sys/times.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdint.h>


namespace cds_basic
{

  /** We define our own types for uchar, uint and ulong. */
  typedef unsigned char cds_uchar;
  typedef unsigned short cds_ushort;
  typedef unsigned int cds_uint;
#ifdef __LP64__
  typedef unsigned long cds_ulong;
#else
  typedef unsigned long long cds_ulong;
#endif
  
  /** Libcds works with words represented by the type cds_word.
      Note that positions are also represented with this type.
   */
  typedef cds_ulong cds_word;

  /** Number of bits in a word in libcds. */
  #define W (8*sizeof(cds_word))
  #define WW (16*sizeof(cds_word))
  
  /** Number of bytes in a word in libcds. */
  #define BW (sizeof(cds_word))
    
  /** Maximum values for different types. */
  #define MAX_CDS_UCHAR (((cds_word)1)<<(8*sizeof(cds_uchar)))
  #define MAX_CDS_UINT (((cds_word)1)<<(8*sizeof(cds_uint)))
  #define MAX_CDS_USHORT (((cds_word)1)<<(8*sizeof(cds_ushort)))
  #define MAX_CDS_ULONG (((cds_word)1)<<(8*sizeof(cds_ulong)-6))
  #define MAX_CDS_WORD MAX_CDS_ULONG

  /** We borrow some macros from the kernel :-) */
  #define likely(x)       __builtin_expect((x),1)
  #define unlikely(x)     __builtin_expect((x),0)

  /** Bits required to represent a number between 0 and n */
#if __LP64__
  inline cds_word msb(cds_word n) {
    cds_word ret=0;
    if(unlikely(n==0)) return 0;
    asm("bsr %1, %0" : "=r" (ret) : "r" (n));
    return ret+1;
  }
#else
  inline cds_word msb(cds_word n) {
    cds_uint ret=0;
    if(unlikely(n==0)) return 0;
    cds_uint v=*(1+(uint*)&n);
    if(v==0) {
      v = *((uint*)&n);
      assert(v!=0);
      asm("bsrl %1, %0" : "=r" (ret) : "r" (v));
      return ret+1;
    }
    asm("bsrl %1, %0" : "=r" (ret) : "r" (v));
    return ret+1+32;
  }
#endif

  /** Counts the number of 1s in x 
   * @param x cds_uint type
   *
   inline cds_word popcount32(const cds_uint x) {
    return __popcount_tab[(x >>  0) & 0xff]  + __popcount_tab[(x >>  8) & 0xff]
      + __popcount_tab[(x >> 16) & 0xff] + __popcount_tab[(x >> 24) & 0xff];
      }*/
  #define popcount32 __builtin_popcount

  /** Counts the number of 1s in x 
   * @param x cds_word type
   */
#if __LP64__
  #define popcount __builtin_popcountl
#else
  inline cds_word popcount(const cds_word x) {
    cds_uint * ptr = (cds_uint*)&x;
    return popcount32(*ptr)+popcount32(*(ptr+1));
  }
#endif


  /** Obtaines the index of the least significant 1-bit of x>1 */
  inline cds_word lsb(cds_word x) {
    if(unlikely(x==0)) {
      return sizeof(cds_word)*8;
    }
    return __builtin_ffsl(x)-1;
  }

  /** Bunch of functions to set and get individual bits from an array of words. */

  /** Retrieve a given index from array A where every value uses len bits
   * @param e Array
   * @param p position
   */  
  inline bool bitget(const cds_word *e, const cds_word p) {
    return 0 != ((e[p/W] >> (p%W)) & 1);
  }

  /** Retrieve a given index from array A where every value uses len bits
   * @param e Array
   * @param p position
   */  
  inline void bitone(cds_word *e, const cds_word p) {
    //std::cout << p%W << " " << p/W <<  std::endl;
    e[p/W] |= ((cds_word)1<<(p%W));
  }

  /** Retrieve a given index from array A where every value uses len bits
   * @param e Array
   * @param p position
   */  
  inline void bitzero(cds_word *e, const cds_word p) {
    e[p/W] &= ~((cds_word)1<<(p%W));
  }

  /** Retrieve a given index from array A where every value uses len bits
   * @param e Array
   * @param p positions
   * @param bit value
   */  
  inline void bitset(cds_word *e, const cds_word p, const bool bit=true) {
    if(bit)
      bitone(e,p);
    else
      bitzero(e,p);
  }

  /** Functions to set and retrieve sequences of bits from a given array. */

  /** Obtains the number of words required to represent n elements of b bits each
   */
  inline cds_word words_length(cds_word n, cds_word b) {
    return (n*b+W-1)/W;
  }

  /** Retrieve a given bitsequence from array A
   * @param A Array
   * @param ini Starting position
   * @param fin Retrieve until end-1
   */
  inline cds_word get_var_field(const cds_word *A, const cds_word ini, const cds_word fin) {
    if(ini==fin+1) return 0;
    cds_word i=ini/W, j=ini-W*i;
    cds_word result;
    cds_word len = (fin-ini+1);
    if (j+len <= W) {
      result = (A[i] << (W-j-len)) >> (W-len);
    } else {
      result = A[i] >> j;
      result = result | (A[i+1] << (WW-j-len)) >> (W-len);
    }
    return result;
  }

  /** Stores a given bitsequence into array A
   * @param A Array
   * @param ini Starting position
   * @param fin Store until end-1
   * @param x Value to be stored
   */
  inline void set_var_field(cds_word *A, const cds_word ini, const cds_word fin, const cds_word x) {
    if(ini==fin+1) return;
    cds_word i=ini/W, j=ini-i*W;
    cds_word len = (fin-ini+1);
    cds_word mask = ((j+len) < W ? ~(cds_word)0 << (j+len) : 0)
      | ((W-j) < W ? ~(cds_word)0 >> (W-j) : 0);
    A[i] = (A[i] & mask) | x << j;
    if (j+len>W) {
      mask = ((~(cds_word)0) << (len+j-W));
      A[i+1] = (A[i+1] & mask)| x >> (W-j);
    }
  }

  /** Retrieve a given index from array A where every value uses len bits
   * @param A Array
   * @param len Length in bits of each field
   * @param index Position to be retrieved
   */
  inline cds_word get_field(const cds_word *A, const cds_word len, const cds_word index) {
    if(len==0) return 0;
    cds_word ilen = index*len;
    //return get_var_field(A,ilen,ilen+len-1);
    cds_word i=ilen/W;
    cds_word j=ilen-W*i;
    cds_word result;
    if (j+len <= W)
      result = (A[i] << (W-j-len)) >> (W-len);
    else {
      result = A[i] >> j;
      result = result | (A[i+1] << (WW-j-len)) >> (W-len);
    }
    return result;
  }

  /** Store a given value in index into array A where every value uses len bits
   * @param A Array
   * @param len Length in bits of each field
   * @param index Position to store in
   * @param x Value to be stored
   */
  inline void set_field(cds_word *A, const cds_word len, const cds_word index, const cds_word x) {
    if(len==0) return;
    cds_word i=index*len/W, j=index*len-i*W;
    cds_word mask = ((j+len) < W ? ~(cds_word)0 << (j+len) : 0)
      | ((W-j) < W ? ~(cds_word)0 >> (W-j) : 0);
    A[i] = (A[i] & mask) | x << j;
    if (j+len>W) {
      mask = ((~(cds_word)0) << (len+j-W));
      A[i+1] = (A[i+1] & mask)| x >> (W-j);
    }
  }  

  /** Retrieve a given index from array A where every value uses 4 bits
   * @param A Array
   * @param index Position to be retrieved
   */
  inline cds_word get_field4(const cds_word *A, const cds_word index) {
    return get_field(A,4,index);
    cds_word i=index/8, j=(index&0x7)<<2;
    return (A[i] << (28-j)) >> (28);
  }

  /** Retrieve a given index from array A where every value uses 8 bits
   * @param A Array
   * @param index Position to be retrieved
   */
  inline cds_word get_field8(const cds_word *A, const cds_word index) {
    const cds_uchar * B = (const cds_uchar*)A;
    return B[index];
  }

  /** Retrieve a given index from array A where every value uses 16 bits
   * @param A Array
   * @param index Position to be retrieved
   */
  inline cds_word get_field16(const cds_word *A, const cds_word index) {
    const cds_ushort * B = (const cds_ushort*)A;
    return B[index];
  }

  /** Retrieve a given index from array A where every value uses 32 bits
   * @param A Array
   * @param index Position to be retrieved
   */
  inline cds_word get_field32(const cds_word *A, const cds_word index) {
    const cds_uint * B = (const cds_uint *)A;
    return B[index];
  }

};

#include <libcds/cdsexception.h>
#include <libcds/time.h>
#include <libcds/io.h>


