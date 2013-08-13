#!/usr/bin/env python
import sys
fp = open(sys.argv[1])
fpout = open(sys.argv[2], "wb")

for line in fp:
	temp = line.strip().split()
	fpout.writelines("%s\t%s\n" % (temp[1][:2], temp[0]))
