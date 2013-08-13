import sys
fp = open(sys.argv[1])

for line in fp:
	l = str(len(line.strip().decode("gb18030")))
	print l, line.strip()
