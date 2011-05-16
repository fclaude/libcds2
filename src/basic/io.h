/**
 *    io.h
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

#pragma once

#include<libcds/libcds.h>
#include<libcds/cdsexception.h>

namespace cds_basic
{

  /** Splits a string into a vector of strings using delim as delimitator.
   */
  inline vector<string> tokenize(string str, char delim) {
    vector<string> tokens;
    string::size_type last_pos = 0;
    string::size_type pos = str.find_first_of(delim);
    while(pos!=string::npos) {
      if(last_pos<pos)
	tokens.push_back(str.substr(last_pos,pos-last_pos));
      last_pos = pos+1;
      if(last_pos >= str.length()) break;
      pos = str.find_first_of(delim,pos+1);
    }
    if(last_pos<str.length())
      tokens.push_back(str.substr(last_pos));
    return tokens;
  }

  /** Converts a string into another type (cds_word, cds_uint, cds_uchar, ...).
   */
  template <typename T> T transform_string(const string & s) {
    stringstream ss;
    ss << s;
    T ret;
    ss >> ret;
    return ret;
  }

  /** Saves a value into an ofstream.
   */
  template <typename T> void save_value(ostream & out, const T val) {
    assert(out.good());
    out.write((char*)&val,sizeof(T));
  }

  /** Loads a value from an ifstream.
   */
  template <typename T> T load_value(istream & in) {
    assert(in.good());
    T ret;
    in.read((char*)&ret,sizeof(T));
    return ret;
  }

  /** Saves len values into an ofstream.
   */
  template <typename T> void save_value(ostream & out, const T * val, const cds_word len) {
    assert(out.good());
    out.write((char*)val,len*sizeof(T));
  }
  
  /** Loads len values from an ifstream.
   */
  template <typename T> T * load_value(istream & in, const cds_word len) {
    assert(in.good());
    T * ret = new T[len];
    in.read((char*)ret,len*sizeof(T));
    return ret;
  }
  
  /** loads a file, stores the length in slen. 
   */
  template <typename T> T * load_file(const string & name, cds_word & slen) {
    ifstream in(name.c_str());
    if(!in.good()) throw CDSException("Error opening file");

    in.seekg(0,ios_base::end);
    size_t len = in.tellg();
    in.seekg(0,ios_base::beg);
    
    if(len%sizeof(T)) {
      in.close();
      throw CDSException("Error, invalid input");
    }

    slen = len/sizeof(T);
    T * ret = load_value<T>(in,slen);
    in.close();
    return ret;
  }

};
