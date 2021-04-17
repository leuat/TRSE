#!/usr/bin/env python
# -*- coding: utf-8 -*-

#    TimTile (c) 2021  Žarko Živanov

#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.

#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.

#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.

from __future__ import print_function
from __future__ import division

VERSION="1.0"

import sys
import os
import argparse
import array
import png      # python-png / python3-png / pypng

###############################################
#               Common functions
###############################################

# returns the size of an open file
def flen(f):
    return os.fstat(f.fileno()).st_size

# prints error message and exits the program
def error(s):
    print("\nERROR: {0}\n".format(s))
    exit(1)

###############################################
#           Argument parser settings
###############################################
about = """TimFont %s (c) 2021  Zarko Zivanov

This program is used to convert PNG into TIM-011
tiles used with its Small C graphics library. Use
provided template for drawing, or create your own.
Be sure to use 8-bit grayscale format. Only colors
recognised as pixels are 40h, 80h and FFh which
represent the colors 01b, 10b and 11b on TIM-011.
All other colors will be treated as black (00b).

For each PNG file a separate '.h' file with DB
definitions of each tile will be created. That file
can be directly imported into Small C programs.""" % (VERSION)

epilog="""
First tile is considered to be a color picker and it
is not converted. All tiles that consist of only black
pixels also are not converted. You can make PNG of any
size, the program will go row by row of 16x16 tiles and
do the conversion.

Be sure that program you use for editing PNG won't
change its format: 1 byte per pixel, greyscale, no
alpha channel, no layers or anything else. Photoshop
users should use the "Flatten Layers" option before
saving their PNG file. GIMP users should export the
image as "8bpc GRAY"."""

parser = argparse.ArgumentParser(description=about, formatter_class=argparse.RawDescriptionHelpFormatter,epilog=epilog)
parser.add_argument('file', metavar='file', nargs='*', default="", help='(path to) PNG file')

# parse command line
args = parser.parse_args()

# recognised colors - all others will be color 00 - black
colors = {0x00:0x00, 0x40:0x01, 0x80:0x02, 0xFF:0x03}

if len(args.file) == 0:
    error("At least one PNG file name is required. Add '-h' for help.")


for pngFile in args.file:
    if not os.path.exists(pngFile):
        error("File '%s' not found!" % pngFile)
    if not pngFile[-4:] in [".png", ".PNG"]:
        error("Image '%s' must have a 'png' extension!" % pngFile)

    # read PNG file
    pngObject = png.Reader(pngFile)
    pngDirect = pngObject.asDirect()
    tileMatrix = list(pngDirect[2])

    tilesW = len(tileMatrix[0]) // 8
    tilesH = len(tileMatrix) // 8
    binName = "%s.bin" % pngFile[:-4]
    print("Creating '%s', %dx%d chars found ..." % (binName, tilesW, tilesH))

    binData = array.array('B')
    for tileY in range(tilesH):
        for tileX in range(tilesW):
            for y in range(tileY*8, tileY*8+8):
                byte = 0
                for x in range(tileX*8, tileX*8+8):
                    byte <<= 1
                    if tileMatrix[y][x] != 0:
                        byte |= 1
                binData.append(byte)
    # open BIN file for writing
    binFile = open(binName, "wb")
    binFile.write(binData)
    binFile.close()
    print("%s created." % binName)

