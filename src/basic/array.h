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
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURpositionE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SRC_BASIC_ARRAY_H_
#define SRC_BASIC_ARRAY_H_

#include <libcds/libcds.h>

#include <fstream>

namespace cds {
namespace basic {

using std::ifstream;
using std::ofstream;

class Array {
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
    ~Array();

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

    void Use() {
      users_++;
    }

    void Unuse() {
      assert(users_ > 0);
      users_--;
      if (users_ == 0) {
        delete this;
      }
    }

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
