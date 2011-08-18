/**
 *    array.cpp
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

#include <libcds/array.h>

namespace cds_basic
{

    Array::Array(ifstream & input) {
        length = load_value<cds_word>(input);
        maxValue = load_value<cds_word>(input);
        bitsPerItem = load_value<cds_word>(input);
        uintLength = load_value<cds_word>(input);
        data = load_value<cds_word>(input,uintLength);
	use();
    }

    Array::Array(cds_word n, cds_word bpe) {
        length = n;
        maxValue = ((cds_word)1<<bpe)-1;
        initData();
	use();
    }

  /*Array::Array(cds_word * A, cds_word n, cds_word bpe) {
        maxValue = 0;
        if(bpe==0) {
            for(cds_word k=0;k<n;k++)
                maxValue = max(maxValue,A[k]);
        }
        else {
	  maxValue = ((cds_word)1<<(bpe))-1;
        }
        length = n;
        initData();
        assert(bpe==0 || bitsPerItem==bpe);
        for(cds_word k=0;k<n;k++) {
            assert(A[k] <= maxValue);
            setField(k, A[k]);
        }
	use();
	}*/

    Array::Array(cds_word * A, cds_word i, cds_word j, cds_word bpe) {
        maxValue = 0;
        if(bpe==0) {
            for(cds_word k=i;k<=j;k++)
                maxValue = max(maxValue,A[k]);
        }
        else {
	  maxValue = ((cds_word)1<<(bpe))-1;
        }
        length = j-i+1;
        initData();
        assert(bpe==0 || bitsPerItem==bpe);
        for(cds_word k=i;k<=j;k++) {
            assert(A[k] <= maxValue);
            setField(k-i, A[k]);
        }
	use();
    }

    Array::~Array() {
        delete [] data;
    }

    void Array::save(ofstream & out) const
    {
        save_value(out,length);
        save_value(out,maxValue);
        save_value(out,bitsPerItem);
        save_value(out,uintLength);
        save_value(out,data,uintLength);
    }

    void Array::initData() {
        bitsPerItem = msb(maxValue);
        uintLength = words_length(length,bitsPerItem);
	//cout << "uintLength=" << uintLength << " bitsPerItem=" << bitsPerItem << endl;
        data = new cds_word[uintLength];
        for(cds_word i=0;i<uintLength;i++)
            data[i] = 0;
    }
};

