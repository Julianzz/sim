#!/usr/bin/env python
import os
import sys

def makeWSDict(path, wordDict):
	fp_temp  = open(path)
	for line in fp_temp:
		temp = line.rstrip('\r\n').split('\t')
		for word in temp:
			if len(word) == 0:
				continue
			wordDict[word] = len(word.decode("GB18030"))
		
def makeDirDict(dirname, wordDict):
	for name in os.listdir(dirname):
		path = dirname.rstrip("/") + "/" + name
		makeWSDict(path, wordDict)
		
def makeSpec(path, wordDict):
	fp_temp = open(path)
	for line in fp_temp:
		temp = line.rstrip('\r\n').split('\t')
		if len(temp[0]) == 0:
			continue
		wordDict[temp[0]] = len(temp[0].decode("GB18030"))
		
def fileout(path, wordDict):
	fp = open(path, "wb")
	for key, value in wordDict.items():
		fp.writelines("%s %s\n" % (value, key))
	fp.close()
	
def out(wordDict):
	for key, value in wordDict.items():
		print "%s %s" % (value, key)
		
def outfile(wordDict, path):
	if len(wordDict) > 0:
		fp = open(path, "wb")
		for key, value in wordDict.items():
			fp.writelines("%s %s\n" % (value, key))
		fp.close()
		
def main(datadir, path):
	dirList = ["typeword", "unlackword"]
	fileList = ["delBigram.dic", "door.dic", 
			"DPMark.dic", "inBracketMark.dic",
			"smName.dic", "spec.dic", "trans.word.dic"]
	specFileList = ["obj.dic", ]
	
	datadir = datadir.rstrip("/") + "/"
	wordDict = {}
	
	for path in fileList:
		path = datadir + path
		makeWSDict(path, wordDict)
		
	for dirname in dirList:
		path = datadir + path
		makeDirDict(dirname, wordDict)
		
	for path in specFileList:
		path = datadir + path
		makeSpec(path, wordDict)
		
	outfile(wordDict, path)
		
if __name__ == "__main__":
	import sys
	datadir = sys.argv[1]
	path = sys.argv[2]
	main(datadir, path)