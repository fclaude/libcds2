/**
 *    cdsexception.h
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

#ifndef SRC_BASIC_CDSEXCEPTION_H_
#define SRC_BASIC_CDSEXCEPTION_H_

#include <libcds/libcds.h>

#include <string>

namespace cds {
namespace basic {
using std::string;

class CDSException {
  public:
    explicit CDSException(const string &s) {
      msg = s;
    }

    string what() {
      return msg;
    }

  protected:
    string msg;
};
};
};

#endif  // SRC_BASIC_CDSEXCEPTION_H_
