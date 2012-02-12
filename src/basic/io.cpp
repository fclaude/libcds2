/**
 *    io.cpp
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

#include <libcds/io.h>

#include <string>
#include <vector>

namespace cds {
namespace basic {

using std::string;
using std::vector;

vector<string> Tokenize(const string &str, const char delim) {
  vector<string> tokens;
  string::size_type last_pos = 0;
  string::size_type pos = str.find_first_of(delim);
  while (pos != string::npos) {
    if (last_pos < pos) {
      tokens.push_back(str.substr(last_pos, pos - last_pos));
    }
    last_pos = pos + 1;
    if (last_pos >= str.length()) {
      break;
    }
    pos = str.find_first_of(delim, pos + 1);
  }
  if (last_pos < str.length()) {
    tokens.push_back(str.substr(last_pos));
  }
  return tokens;
}
};
};
