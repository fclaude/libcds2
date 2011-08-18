/**
 *    bitsequence.h
 *    Copyright (C) 2008  Francisco Claude F.
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
#include <libcds/libcds.h>
#include <libcds/io.h>


namespace cds_static
{
  using namespace std;
  using namespace cds_basic; 

  /** Base class for static bitsequences, contains many abstract functions, 
   *  so this can't be instantiated. It includes base implementations for 
   *  rank0, select0 and select1 based on rank0.
   *
   *  @author Francisco Claude
   */
  class BitSequence
  {

  public:
    virtual ~BitSequence() {};
    
    /** Returns the number of zeros until position i. */
    virtual cds_word rank0(const cds_word i) const;

    /** Returns the position of the i-th zero
     * It returns (size_t)-1 if i=0, length if i>num_zeros, or the 
     * position otherwise.
     */
    virtual cds_word select0(const cds_word i) const;
    
    /** Returns the number of ones until position i. */
    virtual cds_word rank1(const cds_word i) const;
    
    /** Returns the position of the i-th one
     * It returns (cds_word)-1 if i=0, length if i>num_ones, or the 
     * position otherwise. 
     */
    virtual cds_word select1(const cds_word i) const;
    
    /** Return the first position starting at i that contains a 1.
     * In case there are no more ones in the bitsequence, the function 
     * returns the length of the bitmap.
     */
    virtual cds_word selectNext1(const cds_word i) const;
            
    /** Return the first position starting at i that contains a 0.
     * In case there are no more zeros in the bitsequence, the function 
     * returns the length of the bitmap. 
     */
    virtual cds_word selectNext0(const cds_word i) const;
    
    /** Return the first position starting at i moving to the left that 
     *  contains a 1. In case there are no more ones to the left in the 
     *  bitsequence, the function returns length. 
     */
    virtual cds_word selectPrev1(const cds_word i) const;
    
    /** Return the first position starting at i moving to the left that 
     *  contains a 0. In case there are no more zeros to the left in the 
     *  bitsequence, the function returns length. 
     */
    virtual cds_word selectPrev0(const cds_word i) const;
    
    /** Returns the i-th bit. */
    virtual bool access(const cds_word i) const;
    
    /** Returns the length in bits of the bitmap. */
    virtual cds_word getLength() const;
    
    /** Returns how many ones are in the bitstring. */
    virtual cds_word countOnes() const;

    /** Returns how many zeros are in the bitstring. */
    virtual cds_word countZeros() const;
    
    /** Returns the size of the structure in bytes. */
    virtual cds_word getSize() const=0;
    
    /** Stores the bitmap given a file pointer. */
    virtual void save(ofstream & fp) const=0;
    
    /** Reads a bitmap determining the type. */
    static BitSequence * load(ifstream & fp);
    
  protected:
    /** Length of the bitstring. */
    cds_word length;
    /** Number of ones in the bitstring. */
    cds_word ones;
    
  };

};
