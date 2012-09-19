#include <libcds/immutable/mapper.h>
#include <libcds/immutable/mappernone.h>
#include <libcds/immutable/coder.h>
#include <libcds/immutable/codernone.h>
#include <libcds/immutable/bitsequence.h>
#include <libcds/immutable/bitsequenceonelevelrank.h>
#include <libcds/libcds.h>
#include <libcds/array.h>
#include <iostream>

using namespace std;
using namespace cds::basic;
using namespace cds::immutable;

int main() {
	// cds::basic::PrintBinary(35);
	cds_word N = 10000000;
	Array * a = Array::Create(N, cds::basic::msb(N+1));
	a->Use();
	
	for (cds_word i = 0; i < N; i++) {
		a->SetField(i, i+1);
	}
	for (cds_word i = 0; i < N; i++) {
		if (a->GetField(i) != i+1) {
			cout << "Error, values do not match" << endl;
		}
	}
	cout << "Size for a: " << a->GetSize();
	a->Unuse();
	
	// Array *a2 = Array::Create(N, 1);
	// a2->Use();
	// for (cds_word i = 0; i < N; i++) {
	// 	if (i%2) {
	// 		a2->SetField(i, 1);
	// 	}
	// }
	// cout << "Bitmap of length " << N << " requires " << a2->GetSize() << " bytes" << endl;
	// BitSequenceOneLevelRank *b = new BitSequenceOneLevelRank(a2, 20);
	// for (cds_word i = 0; i < N; i++) {
	// 	if (b->Access(i) != (a2->GetField(i)>0)) {
	// 		cout << "Error, values do not match (" << b->Access(i) << ", " << a2->GetField(i) << ")" << endl;
	// 	}
	// }
	// a2->Unuse();
	// b->Unuse();
	// cout << "We are done!" << endl;
}    
