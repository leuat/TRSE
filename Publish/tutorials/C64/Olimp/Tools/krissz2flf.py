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
    flfName = prgName[:-4] + ".flf"
else:
    usage()

def hex2array(hexstring):
    hexstring = hexstring.rsplit()
    result = []
    for num in hexstring:
        result.append(int(num,16))
    return result

FLF_Header = """
46 4c 55 46 46 36 34 02 00 00 00 07 00 00 00 28
19 01 00 00 00 00 00 00 00 00 00 00 00
"""

FLF_Footer = """4d 00 75
00 6c 00 74 00 69 00 63 00 6f 00 6c 00 18 3a 72
65 73 6f 75 72 63 65 73 2f 63 68 61 72 61 63 74
65 72 2e 72 6f 6d 40 45 00 00 00 00 00 00 01 00
00 01 01 00 00 01 00 28 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 01 02 03 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00
"""

prgFile = open(prgName, "rb")
prgFile.seek(0x2001)
screenChr = array.array('B')
screenChr.fromfile(prgFile, 40*25)
prgFile.seek(0x2001+1000)
screenCol = array.array('B')
screenCol.fromfile(prgFile, 40*25)
prgFile.close()

flfHeader = array.array('B', hex2array(FLF_Header))
flfFooter = array.array('B', hex2array(FLF_Footer))

binFile = open(flfName, "wb")
binFile.write(flfHeader)
binFile.write(screenCol)
binFile.write(screenChr)
binFile.write(flfFooter)
binFile.close()

