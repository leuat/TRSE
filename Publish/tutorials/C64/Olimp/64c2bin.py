#!/usr/bin/python3

import sys
import array

def usage():
    print("Usage: %s <64c file>")
    exit(1)

if len(sys.argv) != 2:
    usage()

prgName = sys.argv[1]
if prgName[-4:] in [".64c", ".64C"]:
    binName1 = prgName[:-4] + "-1.bin"
    binName2 = prgName[:-4] + "-2.bin"
else:
    usage()

prgFile = open(prgName, "rb")

#prgFile.seek(0x2)
screen = array.array('B')
screen.fromfile(prgFile, 1024)

binFile = open(binName1, "wb")
binFile.write(screen)
binFile.close()

screen = array.array('B')
screen.fromfile(prgFile, 1024)

binFile = open(binName2, "wb")
binFile.write(screen)
binFile.close()

prgFile.close()

