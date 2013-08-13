#!/usr/bin/env python
#coding: GB18030

import sys

exWordList = ["ҩ��", "����", "�Ƶ�", "������", "�õ�", "�̵�", \
              "������", "���", "���µ�", "�ľߵ�", "ר����", "�տ���", \
              "רӪ��", "Ь��", "����", "������"]
puncList = [' ', '-', '&', '.', ':', '��', ':', "��", ]

def filte(line):
    line = line.rstrip("\r\n")
    if len(line.decode("GB18030")) == 1:
        return
    
    if line[-2:] != "��":
        return
    
    if line[0] in "0123456789":
        return
    
    if line[0:2] in "һ�����������߰˾�":
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


