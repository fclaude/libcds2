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

#ifndef _MEMORYMANAGER_H
#define _MEMORYMANAGER_H

#include <libcds/libcds.h>
#include <libcds/cdsexception.h>
#include <cassert>

using cds::basic::cds_word;
using cds::basic::BitGet;
using cds::basic::BitSet;
using cds::basic::CDSException;
using cds::basic::WordsLength;
using cds::basic::kPointerSize;

namespace cds {
namespace dynamic {

typedef struct __Block {
	cds_word *buffer;
	__Block *next;
} Block;

// This stores the pointer to the block, its offset
// and the block length
typedef struct __MemoryAddress {
	unsigned int offset;
	unsigned int len;
	Block *ptr;
} MemoryAddress;

inline void SetData(cds_word *data, Block *b, cds_word len, cds_word offset) {
	cds_word off_in_block = len * offset + (1 + offset) * sizeof(MemoryAddress*) * 8;
	for (cds_word j = 0; j < len; j++) {
		if (BitGet(data, j)) {
			BitSet(b->buffer, j + off_in_block, 1);
		} else {
			BitSet(b->buffer, j + off_in_block, 0);
		}
	}
}

inline void GetData(Block *b, cds_word len, cds_word offset, cds_word *data) {
	cds_word off_in_block = len * offset + (1 + offset) * sizeof(MemoryAddress*) * 8;
	for (cds_word j = 0; j < len; j++) {
		if (BitGet(b->buffer, j + off_in_block)) {
			BitSet(data, j, 1);
		} else {
			BitSet(data, j, 0);
		}
	}
}

inline MemoryAddress *GetPtr(Block *b, cds_word len, cds_word offset) {
	MemoryAddress *ptr;
	cds_word off_in_block = len * offset + offset * sizeof(MemoryAddress*) * 8;
	for (cds_word j = 0; j < sizeof(MemoryAddress*) * 8; j++) {
		if (BitGet(b->buffer, j + off_in_block)) {
			BitSet((cds_word*)&ptr, j, 1);
		} else {
			BitSet((cds_word*)&ptr, j, 0);
		}
	}
	return ptr;
}

inline MemoryAddress *SetPtr(MemoryAddress *ptr, Block *b, cds_word len, cds_word offset) {
	cds_word off_in_block = len * offset + offset * sizeof(MemoryAddress*) * 8;
	for (cds_word j = 0; j < sizeof(MemoryAddress*) * 8; j++) {
		if (BitGet((cds_word*)&ptr, j)) {
			BitSet(b->buffer, j + off_in_block, 1);
		} else {
			BitSet(b->buffer, j + off_in_block, 0);
		}
	}
	return ptr;
}

inline void MoveBlock(Block *b1, cds_word len, cds_word offset1, Block *b2, cds_word offset2) {
	cds_word *buff = new cds_word[WordsLength(1, len)];
	GetData(b1, len, offset1, buff);
	SetData(buff, b2, len, offset2);
	MemoryAddress *ptr2 = GetPtr(b2, len, offset2);
	MemoryAddress *ptr1 = GetPtr(b1, len, offset1);
	SetPtr(ptr1, b2, len, offset2);
	ptr1->ptr = ptr2->ptr;
	ptr1->offset = ptr2->offset;
	ptr2->offset = (unsigned) -1;
	delete []buff;
}

class MemoryManager {
public:
	MemoryManager(cds_word b, cds_word cell_size) {
		blocks_ = new Block*[b + 1];
		counters_ = new cds_word[b + 1];
		for (cds_word i = 0; i < b + 1; i++) {
			blocks_[i] = NULL;
			counters_[i] = 0;
		}
		b_ = b;
		cell_size_ = cell_size;
	}

	~MemoryManager() {
		// we assume the user freed everything
		for (cds_word i = 0; i < b_ + 1; i++)
			assert(counters_[i] == 0);
		delete [] blocks_;
		delete [] counters_;
	}

	MemoryAddress *Alloc(cds_word len) {
		if (len < b_ || len > 2*b_) {
			throw CDSException("Invalid alloc size");
		}
		MemoryAddress *ptr = new MemoryAddress();
		cds_word i = len - b_;
		cds_word real_cell_size = (cell_size_ / len);
		cds_word offset = counters_[i] % real_cell_size;
		if (offset == 0) {
			Block *b = new Block();
			b->buffer = new cds_word[WordsLength(real_cell_size * len + real_cell_size * sizeof(MemoryAddress*) * 8,1)];
			b->next = blocks_[i];
			blocks_[i] = b;
		}
		ptr->ptr = blocks_[i];
		ptr->offset = offset;
		ptr->len = len;
		SetPtr(ptr, blocks_[i], len, offset);
		assert(GetPtr(blocks_[i], len, offset) == ptr);
		counters_[i]++;
		return ptr;
	}

	void Free(MemoryAddress *addr) {
		cds_word i = addr->len - b_;
		assert(counters_[i] > 0);
		cds_word real_cell_size = (cell_size_ / addr->len);
		cds_word offset = (counters_[i] - 1) % real_cell_size;
		MoveBlock(blocks_[i], addr->len, offset, addr->ptr, addr->offset);
		if (offset == 0) {
			Block * b = blocks_[i];
			blocks_[i] = blocks_[i]->next;
			delete [] b->buffer;
			delete b;
		}
		counters_[i]--;
	}

	void GetData(MemoryAddress *addr, cds_word *buff) {
		cds::dynamic::GetData(addr->ptr, addr->len, addr->offset, buff);
	}

	void SetData(MemoryAddress *addr, cds_word *buff) {
		cds::dynamic::SetData(buff, addr->ptr, addr->len, addr->offset);
	}

protected:
	Block **blocks_;
	cds_word b_, cell_size_, *counters_;
};
};
};

#endif  // _MEMORYMANAGER_H
