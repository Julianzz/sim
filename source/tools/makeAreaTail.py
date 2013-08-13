#!/usr/bin/env python
#coding: GB18030

import sys

exWordList = ["药店", "饭店", "酒店", "便利店", "旅店", "商店", \
              "西饼店", "书店", "故事店", "文具店", "专卖店", "烧烤店", \
              "专营店", "鞋店", "饼店", "花果店"]
puncList = [' ', '-', '&', '.', ':', '。', ':', "：", ]

def filte(line):
    line = line.rstrip("\r\n")
    if len(line.decode("GB18030")) == 1:
        return
    
    if line[-2:] != "店":
        return
    
    if line[0] in "0123456789":
        return
    
    if line[0:2] in "一二三四五六七八九":
        return
    
    for w in exWordList:
        if line.endswith(w):
            return
        
    line = line.lower()
    for p in puncList:
        line = line.replace(p, "")
    print line
    
def main():
    path = sys.argv[1]
    fp = open(path)
    for line in fp:
        filte(line)
        
if __name__ == "__main__":
    main()


