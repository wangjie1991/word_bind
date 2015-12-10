#!/usr/bin/python
#-*- coding: utf-8 -*-

import os
import sys

def NewBindDict(fnl, fgb, fnb):
    pDic = {}
    while 1:
        gb_line = fgb.readline()
        if not gb_line: break
        gb_line = gb_line.decode('gbk')
        gb_word = gb_line[:gb_line.find('\t')]
        if not pDic.has_key(gb_word):
            pDic[gb_word] = gb_line

    while 1:
        nl_line = fnl.readline()
        if not nl_line: break
        nl_line = nl_line.decode('gbk')
        nl_word = nl_line[:nl_line.find('\t')]
        if pDic.has_key(nl_word):
            fnb.write(pDic[nl_word].encode('gbk'))
        else:
            nb_line = '%s\t10.000000\n' % nl_word
            fnb.write(nb_line.encode('gbk'))


if __name__ == '__main__':

    if len(sys.argv) != 4:
        print 'Usage : python %s new_lexicon general_bind_dict \
                new_bind_dict' % sys.argv[0]
        sys.exit(1)

    if not os.path.exists(sys.argv[1]):
        print 'Error : cannot find ', sys.argv[1]
        sys.exit(1)

    if not os.path.exists(sys.argv[2]):
        print 'Error : cannot find ', sys.argv[2]
        sys.exit(1)

    fnl = open(sys.argv[1], 'r')
    fgb = open(sys.argv[2], 'r')
    fnb = open(sys.argv[3], 'w')

    NewBindDict(fnl, fgb, fnb);

    fnl.close()
    fgb.close()
    fnb.close()


