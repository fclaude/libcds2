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
#include <libcds/io.h>
// #include <libcds/immutable/bitsequenceonelevelrank.h>

#include <algorithm>
#include <fstream>

namespace cds {

    namespace immutable {
        class BitSequenceOneLevelRank;
    }

namespace basic {

using std::ifstream;
using std::ofstream;
using std::max;

class Array : public ReferenceCounted {
  public:
    /** Destroys the array */
    virtual ~Array() {}

    /** Retrieves Array[position]
     * @paran position positionition
     * @return Array[position]
     */
    virtual cds_word GetField(const cds_word position) const = 0;
    cds_word operator[](const cds_word position) const {
      return GetField(position);
    }

    /** Assigns v to Array[position]
     * @paran position positionition
     * @param v value
     * @return Array[position]
     */
    virtual cds_word SetField(const cds_word position, const cds_word v) = 0;

    /** Saves the array into a file */
    virtual void Save(ofstream &out) const = 0;

    /** Returns the size of the array in bytes
     */
    virtual cds_word GetSize() const = 0;

    /** Returns the length of the array
     */
    virtual cds_word GetLength() const = 0;

    /** Returns the maximum value in the array
     */
    virtual cds_word GetMax() const = 0;

    /** Returns the position of the first element between ini and fin-1
        that is not less than the value.
        If none, then it return fin.
    */
    virtual cds_word LowerBound(cds_word value, cds_word ini, cds_word fin) const = 0;

    /** Returns the position of the first element that is not less than the value.
        If none, then it return the length.
    */
    virtual cds_word LowerBound(cds_word value) const = 0;

    /** Returns the position of the first element between ini and fin-1
        that is greater than the value.
        If none, then it return fin.
    */
    virtual cds_word UpperBound(cds_word value, cds_word ini, cds_word fin) const = 0;

    /** Returns the position of the first element that is greater than the value.
        If none, then it return fin.
    */
    virtual cds_word UpperBound(cds_word value) const = 0;

    /* Binary search for a value in [ini,fin] and return the position of its first
        appareance. If the element is not present, it return fin.
        */
    virtual cds_word BinarySearch(cds_word value, cds_word ini, cds_word fin) const = 0;

    /* Binary search for a value in [0,length] and return the position of its first
        appareance. If the element is not present, it return fin.
        */
    virtual cds_word BinarySearch(cds_word value) const = 0;

    /** Loads an array from disk */
    static Array *Load(ifstream &input);

    /** Creates an array copying part of a previously existing array
     * @param A source array
     * @param i initial positionition
     * @param j final positionition
     * @param bpe size per field (in bits), use 0 for auto
     */
    static Array *Create(cds_word *A, cds_word i, cds_word j, cds_word bpe = 0);

    /** Creates and array with n elements that can store elements between 0 and 2^{bpe}-1
     * @param n length (in elements) of the array
     * @param bpe size per field (in bits)
     */
    static Array *Create(cds_word n, cds_word bpe);
};

template <cds_word bpe> class ArrayTpl : public Array {
  public:
    /** Reads and ArrayTpl from a file stream
     * @param input input file stream
     */
    explicit ArrayTpl(ifstream &input);

    /** Creates an array copying part of a previously existing array
     * @param A source array
     * @param i initial positionition
     * @param j final positionition
     */
    ArrayTpl(cds_word *A, cds_word i, cds_word j);

    /** Creates and array with n elements that can store elements between 0 and _maxValue
     * @param n length (in elements) of the array
     */
    explicit ArrayTpl(cds_word n);

    /** Destroys the array */
    virtual ~ArrayTpl();

    virtual cds_word GetField(const cds_word position) const;

    virtual cds_word SetField(const cds_word position, const cds_word v);

    virtual void Save(ofstream &out) const;

    virtual cds_word GetSize() const;

    virtual cds_word GetLength() const;

    virtual cds_word GetMax() const;

    virtual cds_word LowerBound(cds_word value, cds_word ini, cds_word fin) const;

    virtual cds_word LowerBound(cds_word value) const;

    virtual cds_word UpperBound(cds_word value, cds_word ini, cds_word fin) const;

    virtual cds_word UpperBound(cds_word value) const;

    virtual cds_word BinarySearch(cds_word value, cds_word ini, cds_word fin) const;

    virtual cds_word BinarySearch(cds_word value) const;


    // friend cds_word operator=(const ArrayTplModifier a);
    friend class cds::immutable::BitSequenceOneLevelRank;

  protected:
    /** ArrayTpl where the data is stored */
    cds_word *data_;
    /** Length of the array in number of elements */
    cds_word length_;
    /** Maximum value stored in the array */
    cds_word max_value_;
    /** Length of the array in number of uints */
    cds_word uint_length_;

    cds_word users_;

    /** Initializes the array, all the values are assigned to 0
     */
    void InitData();
};
};
};

#endif  // SRC_BASIC_ARRAY_H_
