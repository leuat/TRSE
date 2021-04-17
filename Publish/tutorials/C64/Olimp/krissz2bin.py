#!/usr/bin/python3

import sys
import array

def usage():
    print("Usage: %s <prg file>")
    exit(1)

if len(sys.argv) != 2:
    usage()

prgName = sys.argv[1]
if prgName[-4:] in [".prg", ".PRG"]:
    binName = prgName[:-4] + ".bin"
else:
    usage()

prgFile = open(prgName, "rb")
prgFile.seek(0x2001)
screen = array.array('B')
screen.fromfile(prgFile, 2000)
prgFile.close()

binFile = open(binName, "wb")
binFile.write(screen)
binFile.close()

