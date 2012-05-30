#!/usr/bin/python

for i in range(64):
	print "\t\tcase %d: return new ArrayTpl<%d>(n);" % (i, i)

for i in range(58):
	print "TEST(Array,Timing%d) {\n\ttestForSpeed(%d);\n}" % (i, i)

for i in range(58):
	print "\t\tcase %d: return ArrayTpl<%d>::Load(input);" % (i, i)

