/**
 *    array.h
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

#include <fstream>
#include <vector>
#include <set>

#include <libcds/libcds.h>

using namespace std;

namespace cds_basic
{

  class Array
  {
  public:
    class ArrayModifier
    {
    protected:
      cds_word * data;
      cds_word len;
      cds_word bitsPerItem;
      cds_word pos;
    public:
    ArrayModifier(cds_word * _data, cds_word _len, cds_word _bitsPerItem, cds_word _pos)
      : data(_data), len(_len), bitsPerItem(_bitsPerItem), pos(_pos) {}
      inline cds_word operator=(const cds_word v) {
	set_field(data, bitsPerItem, pos, v);
	return v;
      }

      inline cds_word getValue() {
	return get_field(data, bitsPerItem,pos);
      }

      inline bool operator==(const ArrayModifier & v) {
	return get_field(data, bitsPerItem,pos) == get_field(v.data,v.bitsPerItem,v.pos);
      }

      inline bool operator!=(const ArrayModifier & v) {
	return get_field(data, bitsPerItem,pos) != get_field(v.data,v.bitsPerItem,v.pos);
      }

      inline bool operator<(const ArrayModifier & v) {
	return get_field(data, bitsPerItem,pos) < get_field(v.data,v.bitsPerItem,v.pos);
      }

      inline bool operator<=(const ArrayModifier & v) {
	return get_field(data, bitsPerItem,pos) <= get_field(v.data,v.bitsPerItem,v.pos);
      }

      inline bool operator>(const ArrayModifier & v) {
	return get_field(data, bitsPerItem,pos) > get_field(v.data,v.bitsPerItem,v.pos);
      }

      inline bool operator>=(const ArrayModifier & v) {
	return get_field(data, bitsPerItem,pos) >= get_field(v.data,v.bitsPerItem,v.pos);
      }

      inline operator cds_word() {
	return get_field(data, bitsPerItem, pos);
      }

    };
  public:
    /** Reads and Array from a file stream
     * @param input input file stream
     */
    Array(ifstream & input);

    /** Creates an array copying part of a previously existing array
     * @param A source array
     * @param i initial position
     * @param j final position
     * @param bpe bits per element (0 uses the max in A[i,j]
     */
    Array(cds_word * A, cds_word i, cds_word j, cds_word bpe = 0);

    /** Creates and array with n elements that can store elements between 0 and _maxValue
     * @param n length (in elements) of the array
     * @param _maxValue maximum value that could be stored in the array
     */
    Array(cds_word n, cds_word bpe);

    /** Destroys the array */
    ~Array();

    /** Retrieves Array[pos]
     * @paran pos position
     * @return Array[pos]
     */
    inline cds_word getField(const cds_word pos) const
    {
      assert(pos<length);
      return get_field(data, bitsPerItem, pos);
    }

    /** Assigns v to Array[pos]
     * @paran pos position
     * @param v value
     * @return Array[pos]
     */
    inline cds_word setField(const cds_word pos, const cds_word v) {
      assert(pos<length); assert(v<=maxValue);
      set_field(data, bitsPerItem, pos, v);
      return v;
    }

    /** operator [] for getField
     */
    cds_word const operator[](const cds_word pos) const
    {
      assert(pos<length);
      return get_field(data, bitsPerItem, pos);
    }

    /** Operator [] for modifying array
	This is not thread-safe
     *
    ArrayModifier & operator[](const cds_word pos) {
      assert(pos<length);
      static ArrayModifier ret = ArrayModifier(data,length,bitsPerItem,pos);
      return ret;
      }*/

    /** Saves the array into a file */
    void save(ofstream & out) const;

    /** Returns the size of the array in bytes
     */
    inline cds_word getSize() const
    {
      return sizeof(cds_word)*uintLength+sizeof(cds_word)*4+sizeof(cds_word*);
    }

    /** Returns the length of the array
     */
    inline cds_word getLength() const
    {
      return length;
    }

    /** Returns the maximum value in the array
     */
    inline cds_word getMax() const
    {
      cds_word maxV = 0;   // default max
      for(cds_word i=0; i<length; i++)
	maxV = max(maxV,getField(i));
      return maxV;
    }

    void use() {
      users++;
    }

    void unuse() {
      assert(users>0);
      users--;
      if(users==0)
	delete this;
    }

    //friend cds_word operator=(const ArrayModifier a);
  protected:
    /** Array where the data is stored */
    cds_word * data;
    /** Length of the array in number of elements */
    cds_word length;
    /** Length in bits of each field */
    cds_word bitsPerItem;
    /** Maximum value stored in the array */
    cds_word maxValue;
    /** Length of the array in number of uints */
    cds_word uintLength;

    cds_word users;

    /** Initializes the array, all the values to 0
     */
    void initData();
  };
}


