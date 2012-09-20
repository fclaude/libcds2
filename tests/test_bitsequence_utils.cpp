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

#include <gtest/gtest.h>
#include <libcds/array.h>
#include <libcds/immutable/bitsequence.h>

using cds::immutable::BitSequence;
using cds::basic::Array;
using cds::basic::ArrayTpl;
using cds::basic::cds_word;

Array *CreateRandomBitmap(const cds_word kBitmapLength, const cds_word kOnes, unsigned int seed) {
  Array *ret = Array::Create(kBitmapLength, 1);
  // ret->Use();
  for (cds_word i = 0; i < kOnes;) {
    cds_word pos = rand_r(&seed) % kBitmapLength;
    if (ret->GetField(pos) == 0) {
      ret->SetField(pos, 1);
      i++;
    }
  }
  return ret;
}

void TestBitSequence(BitSequence *model, BitSequence *tested) {
  const cds_word kBitmapLength = model->GetLength();
  const cds_word kOnes = model->CountOnes();
  for (cds_word i = 0; i < kBitmapLength; i++) {
    cds_word exp = model->Rank0(i);
    cds_word rec = tested->Rank0(i);
    ASSERT_EQ(exp, rec);
    bool access;
    rec = tested->Rank0(i, &access);
    bool access_expected = model->Access(i);
    ASSERT_EQ(access_expected, access);
    ASSERT_EQ(exp, rec);
  }
  for (cds_word i = 0; i < kBitmapLength; i++) {
    cds_word exp = model->Rank1(i);
    cds_word rec = tested->Rank1(i);
    ASSERT_EQ(exp, rec);
    bool access;
    rec = tested->Rank1(i, &access);
    bool access_expected = model->Access(i);
    ASSERT_EQ(access_expected, access);
    ASSERT_EQ(exp, rec);
  }
  for (cds_word i = 0; i < kBitmapLength; i++) {
    cds_word exp;
    cds_word rec;
    bool access = tested->Access(i, &rec);
    bool access_expected = model->Access(i, &exp);
    ASSERT_EQ(access_expected, access);
    ASSERT_EQ(exp, rec);
  }
  for (cds_word i = 1; i < kBitmapLength - kOnes + 1; i++) {
    cds_word exp = model->Select0(i);
    cds_word rec = tested->Select0(i);
    ASSERT_EQ(exp, rec);
  }
  for (cds_word i = 1; i < kOnes; i++) {
    cds_word exp = model->Select1(i);
    cds_word rec = tested->Select1(i);
    ASSERT_EQ(exp, rec);
  }
  for (cds_word i = 0; i < kBitmapLength; i++) {
    cds_word exp = model->SelectNext1(i);
    cds_word rec = tested->SelectNext1(i);
    ASSERT_EQ(exp, rec);
    if (exp < model->GetLength()) {
      exp = tested->Select1(tested->Rank1(exp), i, exp);
    } else {
      exp = tested->Select1(tested->Rank1(exp - 1) + 1, i, exp - 1);
    }
    ASSERT_EQ(exp, rec);
  }
  for (cds_word i = 0; i < kBitmapLength; i++) {
    cds_word exp = model->SelectNext0(i);
    cds_word rec = tested->SelectNext0(i);
    ASSERT_EQ(exp, rec);
    if (exp < model->GetLength()) {
      exp = tested->Select0(tested->Rank0(exp), i, exp);
    } else {
      exp = tested->Select0(tested->Rank0(exp - 1) + 1, i, exp - 1);
    }
    ASSERT_EQ(exp, rec);
  }
  for (cds_word i = 0; i < kBitmapLength; i++) {
    cds_word exp = model->SelectPrev0(i);
    cds_word rec = tested->SelectPrev0(i);
    ASSERT_EQ(exp, rec);
  }
  for (cds_word i = 0; i < kBitmapLength; i++) {
    cds_word exp = model->SelectPrev1(i);
    cds_word rec = tested->SelectPrev1(i);
    ASSERT_EQ(exp, rec);
  }
  cds_word exp, rec;
  exp = kOnes;
  rec = tested->CountOnes();
  ASSERT_EQ(exp, rec);
  exp = kBitmapLength - kOnes;
  rec = tested->CountZeros();
  ASSERT_EQ(exp, rec);
}
