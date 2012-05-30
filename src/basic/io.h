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


#ifndef SRC_BASIC_IO_H_
#define SRC_BASIC_IO_H_

#include <libcds/libcds.h>
#include <libcds/cdsexception.h>

#include <vector>
#include <string>
#include <sstream>
#include <fstream>

namespace cds {
namespace basic {

using std::vector;
using std::stringstream;
using std::istream;
using std::ostream;
using std::ifstream;
using std::ofstream;
using std::ios_base;

/** Splits a string into a vector of strings using delim as delimitator.
 */
vector<string> Tokenize(const string &str, const char delim);

/** Converts a string into another type (cds_word, cds_uint, cds_uchar, ...).
 */
template <typename T> T TransformString(const string &s) {
  stringstream ss;
  ss << s;
  T ret;
  ss >> ret;
  return ret;
}

/** Saves a value into an ostream.
 */
template <typename T> void SaveValue(ostream &out, T val) {
  assert(out.good());
  out.write(reinterpret_cast<char *>(&val), sizeof(T));
}

/** Loads a value from an istream.
 */
template <typename T> T LoadValue(istream &in) {
  assert(in.good());
  T ret;
  in.read(reinterpret_cast<char *>(&ret), sizeof(T));
  return ret;
}

/** Saves len values into an ostream.
 */
template <typename T> void SaveValue(ostream &out, T *val, const cds_word length) {
  assert(out.good());
  out.write(reinterpret_cast<char *>(val), length * sizeof(T));
}

/** Loads len values from an istream.
 */
template <typename T> T *LoadValue(istream &in, const cds_word length) {
  assert(in.good());
  T *ret = new T[length];
  in.read(reinterpret_cast<char *>(ret), length * sizeof(T));
  return ret;
}

/** loads a file, stores the length in content_length.
 */
template <typename T> T *LoadFile(const string &name, cds_word &content_length) {
  ifstream in(name.c_str());
  if (!in.good()) {
    throw CDSException("Error opening file");
  }

  in.seekg(0, ios_base::end);
  size_t length = in.tellg();
  in.seekg(0, ios_base::beg);

  if (length % sizeof(T)) {
    in.close();
    throw CDSException("Error, invalid input");
  }

  content_length = length / sizeof(T);
  T *ret = LoadValue<T>(in, content_length);
  in.close();
  return ret;
}

/** Saves a value into an ostream.
 */
template <cds_word> void SaveValue(ostream &out, const cds_word value) {
  assert(out.good());
#if __LP64__
  out.write(reinterpret_cast<char *>(&value), sizeof(cds_word));
#else
  unsigned long long v = value;
  out.write(reinterpret_cast<char *>(&v), sizeof(unsigned long long));
#endif
}

/** Loads a value from an istream.
 */
template <cds_word> cds_word LoadValue(istream &in) {
  assert(in.good());
  cds_word ret;
#if __LP64__
  in.read(reinterpret_cast<char *>(&ret), sizeof(cds_word));
#else
  unsigned long long v;
  in.read(reinterpret_cast<char *>(&v), sizeof(unsigned long long));
  if (v > kMaxCDSWord) {
    throw CDSException("Value too large for the size of the word");
  }
  ret = (cds_word)v;
#endif
  return ret;
}

/** Saves len values into an ostream.
 */
template <cds_word> void SaveValue(ostream &out, const cds_word *values, const cds_word length) {
  assert(out.good());
#if __LP64__
  out.write(reinterpret_cast<char *>(values), length * sizeof(cds_word));
#else
  for (cds_word i = 0; i < length; i++) {
    unsigned long long v = values[i];
    out.write(reinterpret_cast<char *>(&v), sizeof(unsigned long long));
  }
#endif
}

/** Loads len values from an istream.
 */
template <cds_word> cds_word *LoadValue(istream &in, const cds_word length) {
  assert(in.good());
  cds_word *ret = new cds_word[length];
#if __LP64__
  in.read(reinterpret_cast<char *>(ret), length * sizeof(cds_word));
#else
  for (uint i = 0; i < length; i++) {
    ret[i] = LoadValue<cds_word>(in);
  }
#endif
  return ret;
}
};
};

#endif  // SRC_BASIC_IO_H_
