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


#ifndef SRC_BASIC_ARRAY_H_
#define SRC_BASIC_ARRAY_H_

#include <libcds/libcds.h>

#include <fstream>

namespace cds {
namespace basic {

using std::ifstream;
using std::ofstream;

class Array : public ReferenceCounted {
  public:
    /** Reads and Array from a file stream
     * @param input input file stream
     */
    explicit Array(ifstream &input);

    /** Creates an array copying part of a previously existing array
     * @param A source array
     * @param i initial positionition
     * @param j final positionition
     * @param bpe bits per element (0 uses the max in A[i,j]
     */
    Array(cds_word *A, cds_word i, cds_word j, cds_word bpe = 0);

    /** Creates and array with n elements that can store elements between 0 and _maxValue
     * @param n length (in elements) of the array
     * @param _maxValue maximum value that could be stored in the array
     */
    Array(cds_word n, cds_word bpe);

    /** Destroys the array */
    virtual ~Array();

    /** Retrieves Array[position]
     * @paran position positionition
     * @return Array[position]
     */
    inline cds_word GetField(const cds_word position) const {
      assert(position < length_);
      return cds::basic::GetField(data_, bits_per_item_, position);
    }

    /** Assigns v to Array[position]
     * @paran position positionition
     * @param v value
     * @return Array[position]
     */
    inline cds_word SetField(const cds_word position, const cds_word v) {
      assert(position < length_);
      assert(v <= max_value_);
      cds::basic::SetField(data_, bits_per_item_, position, v);
      return v;
    }

    /** operator [] for getField
     */
    cds_word const operator[](const cds_word position) const {
      assert(position < length_);
      return cds::basic::GetField(data_, bits_per_item_, position);
    }

    /** Operator [] for modifying array
    This is not thread-safe
     *
    ArrayModifier & operator[](const cds_word position) {
      assert(position<length);
      static ArrayModifier ret = ArrayModifier(data,length,bits_per_item,position);
      return ret;
      }*/

    /** Saves the array into a file */
    void Save(ofstream &out) const;

    /** Returns the size of the array in bytes
     */
    inline cds_word GetSize() const {
      return sizeof(cds_word) * uint_length_
             + sizeof(cds_word) * 4
             + sizeof(cds_word *);
    }

    /** Returns the length of the array
     */
    inline cds_word GetLength() const {
      return length_;
    }

    /** Returns the maximum value in the array
     */
    cds_word GetMax() const;

    /** Returns the position of the first element between ini and fin-1
        that is not less than the value.
        If none, then it return fin.
    */
    cds_word LowerBound(cds_word value, cds_word ini, cds_word fin) const;

    /** Returns the position of the first element that is not less than the value.
        If none, then it return the length.
    */
    cds_word LowerBound(cds_word value) const;

    /** Returns the position of the first element between ini and fin-1
        that is greater than the value.
        If none, then it return fin.
    */
    cds_word UpperBound(cds_word value, cds_word ini, cds_word fin) const;

    /** Returns the position of the first element that is greater than the value.
        If none, then it return fin.
    */
    cds_word UpperBound(cds_word value) const;

    /* Binary search for a value in [ini,fin] and return the position of its first
        appareance. If the element is not present, it return fin.
        */
    cds_word BinarySearch(cds_word value, cds_word ini, cds_word fin) const;

    /* Binary search for a value in [0,length] and return the position of its first
        appareance. If the element is not present, it return fin.
        */
    cds_word BinarySearch(cds_word value) const;

    class ArrayModifier {
      public:
        ArrayModifier(cds_word *data, cds_word length, cds_word bits_per_item, cds_word position)
          : data_(data), length_(length), bits_per_item_(bits_per_item), position_(position) {}

        inline cds_word operator=(const cds_word v) {
          cds::basic::SetField(data_, bits_per_item_, position_, v);
          return v;
        }

        inline cds_word getValue() {
          return cds::basic::GetField(data_, bits_per_item_, position_);
        }

        inline bool operator==(const ArrayModifier &v) {
          return cds::basic::GetField(data_, bits_per_item_, position_)
                 == cds::basic::GetField(v.data_, v.bits_per_item_, v.position_);
        }

        inline bool operator!=(const ArrayModifier &v) {
          return cds::basic::GetField(data_, bits_per_item_, position_)
                 != cds::basic::GetField(v.data_, v.bits_per_item_, v.position_);
        }

        inline bool operator<(const ArrayModifier &v) {
          return cds::basic::GetField(data_, bits_per_item_, position_)
                 < cds::basic::GetField(v.data_, v.bits_per_item_, v.position_);
        }

        inline bool operator<=(const ArrayModifier &v) {
          return cds::basic::GetField(data_, bits_per_item_, position_)
                 <= cds::basic::GetField(v.data_, v.bits_per_item_, v.position_);
        }

        inline bool operator>(const ArrayModifier &v) {
          return cds::basic::GetField(data_, bits_per_item_, position_)
                 > cds::basic::GetField(v.data_, v.bits_per_item_, v.position_);
        }

        inline bool operator>=(const ArrayModifier &v) {
          return cds::basic::GetField(data_, bits_per_item_, position_)
                 >= cds::basic::GetField(v.data_, v.bits_per_item_, v.position_);
        }

        inline operator cds_word() {
          return cds::basic::GetField(data_, bits_per_item_, position_);
        }

      protected:
        cds_word *data_;
        cds_word length_;
        cds_word bits_per_item_;
        cds_word position_;
    };

    // friend cds_word operator=(const ArrayModifier a);
    friend class BitSequenceOneLevelRank;

  protected:
    /** Array where the data is stored */
    cds_word *data_;
    /** Length of the array in number of elements */
    cds_word length_;
    /** Length in bits of each field */
    cds_word bits_per_item_;
    /** Maximum value stored in the array */
    cds_word max_value_;
    /** Length of the array in number of uints */
    cds_word uint_length_;

    cds_word users_;

    /** Initializes the array, all the values to 0
     */
    void InitData();
};
};
};

#endif  // SRC_BASIC_ARRAY_H_
