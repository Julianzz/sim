#!/usr/bin/env python
#coding: utf8

import sys

fp = open(sys.argv[1])
fpout = open(sys.argv[2], "wb")

for line in fp:
	temp = line.strip().split()
	fpout.writelines(line.strip() + "\t" + "第".decode("utf8").encode("gb18030") + temp[0] + "\n")
