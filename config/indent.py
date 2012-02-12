#!/usr/bin/python

#    copy_header.py
#    Copyright (C) 2011  Francisco Claude F.
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

import os
import sys

if len(sys.argv)!=2:
    print("Usage: %s <file>" % sys.argv[0])
    sys.exit(0)

os.system("astyle --options=./config/astyle.cfg %s\n" % (sys.argv[1]))
