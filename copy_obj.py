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
import shutil

if len(sys.argv)!=2:
    print "usage: " + sys.argv[0] + " <header>"
    os.exit(0)

fname = sys.argv[1]
#print "File name: " + fname
path = fname.split("/")[2:-1]
actual_path = "./obj"
for dir in path:
    if dir=="basic": continue
    actual_path += "/"+dir
    if not os.access(actual_path,os.R_OK):
        #print "mkdir " + actual_path
        os.mkdir(actual_path)
#print "cp "+fname+" "+actual_path
shutil.copy(fname,actual_path)
        
