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


#ifndef SRC_IMMUTABLE_BITSEQUENCE_H_
#define SRC_IMMUTABLE_BITSEQUENCE_H_


#include <libcds/libcds.h>
#include <libcds/io.h>


#include <fstream>

namespace cds {
namespace immutable {

using cds::basic::cds_word;
using std::ifstream;
using std::ofstream;

/** Base class for static bitsequences, contains many abstract functions,
 *  so this can't be instantiated. It includes base implementations for
 *  rank0, select0 and select1 based on rank0.
 *
 *  @author Francisco Claude
 */
class BitSequence : public cds::basic::ReferenceCounted {
  public:
    virtual ~BitSequence() {}

    /** Returns the number of zeros until position i. */
    virtual cds_word Rank0(const cds_word i) const;

    /** Returns the position of the i-th zero
     * It returns (size_t)-1 if i=0, length if i>num_zeros, or the
     * position otherwise.
     */
    virtual cds_word Select0(const cds_word i) const;

    /** Returns the number of ones until position i. */
    virtual cds_word Rank1(const cds_word i) const;

    /** Returns the position of the i-th one
     * It returns (cds_word)-1 if i=0, length if i>num_ones, or the
     * position otherwise.
     */
    virtual cds_word Select1(const cds_word i) const;

    /** Return the first position starting at i that contains a 1.
     * In case there are no more ones in the bitsequence, the function
     * returns the length of the bitmap.
     */
    virtual cds_word SelectNext1(const cds_word i) const;

    /** Return the first position starting at i that contains a 0.
     * In case there are no more zeros in the bitsequence, the function
     * returns the length of the bitmap.
     */
    virtual cds_word SelectNext0(const cds_word i) const;

    /** Return the first position starting at i moving to the left that
     *  contains a 1. In case there are no more ones to the left in the
     *  bitsequence, the function returns length.
     */
    virtual cds_word SelectPrev1(const cds_word i) const;

    /** Return the first position starting at i moving to the left that
     *  contains a 0. In case there are no more zeros to the left in the
     *  bitsequence, the function returns length.
     */
    virtual cds_word SelectPrev0(const cds_word i) const;

    /** Returns the i-th bit. */
    virtual bool Access(const cds_word i) const;

    /** Returns the length in bits of the bitmap. */
    virtual cds_word GetLength() const = 0;

    /** Returns how many ones are in the bitstring. */
    virtual cds_word CountOnes() const;

    /** Returns how many zeros are in the bitstring. */
    virtual cds_word CountZeros() const;

    /** Returns the size of the structure in bytes. */
    virtual cds_word GetSize() const = 0;

    /** Stores the bitmap given a file pointer. */
    virtual void Save(ofstream &fp) const = 0;

    /** Reads a bitmap determining the type. */
    static BitSequence *Load(ifstream &fp);

};
};
};

#endif  // SRC_IMMUTABLE_BITSEQUENCE_H_
