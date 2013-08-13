#!/usr/bin/env python
#coding:GB18030
'''
Created on 2011-11-14

@author: stefanyang
'''

def makeDict(path, outPath):
    fp = open(path)
    fpout = open(outPath, "wb")
    
    resultDict = {}
    
    for line in fp:
        line = line.strip()
        if line != "":
            temp = line.split("\t")
            if resultDict.has_key(temp[0]):
                resultDict[temp[0]].extend(temp[1:])
            else:
                resultDict[temp[0]] = temp[1:]
    for k, v in resultDict.items():
        temp = [k, ] + list(set(v))
        fpout.writelines("\t".join(temp) + "\n")
        
if __name__ == "__main__":
    import sys
    
    path = "states"
    outPath = "trans.word.dic"
    makeDict(path, outPath)
            