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


#ifndef SRC_IMMUTABLE_SEQUENCE_H_
#define SRC_IMMUTABLE_SEQUENCE_H_


#include <libcds/libcds.h>
#include <libcds/io.h>


#include <fstream>

namespace cds {
namespace immutable {

using cds::basic::cds_word;
using std::istream;
using std::ostream;

/** Base class for static sequences, contains many abstract functions,
 *  so this can't be instantiated.
 *
 *  @author Francisco Claude
 */
class Sequence : public cds::basic::ReferenceCounted {
  public:
    virtual ~Sequence() {}

    /** Retrieves the element at position i. */
    virtual cds_word Access(const cds_word i) const;

    /** Retrieves the symbol at position i, and the number of occurrences
     * of the symbol in S[1..i].
     */
    virtual cds_word Access(const cds_word i, cds_word *rank) const;

    /** Counts the number of occurrences of symbol s in S[1..i]. */
    virtual cds_word Rank(const cds_word s, const cds_word i) const;

    /** Computes the position of the j-th occurrence of symbol s. */
    virtual cds_word Select(const cds_word s, const cds_word j) const;

    /** Returns the length in bits of the sequence. */
    virtual cds_word GetLength() const = 0;

    /** Returns the number of occurrences of a given symbol s. */
    virtual cds_word Count(const cds_word s) const;

    /** Returns the maximum value for a symbol (size of the alphabet). */
    virtual cds_word GetSigma() const;

    /** Returns the size of the structure in bytes. */
    virtual cds_word GetSize() const = 0;

    /** Stores the sequence given an output stream. */
    virtual void Save(ostream &fp) const = 0;

    /** Reads a sequence determining the type. */
    static Sequence *Load(istream &fp);
};
};
};

#endif  // SRC_IMMUTABLE_SEQUENCE_H_
