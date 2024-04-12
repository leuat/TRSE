# TimTris v0.9

This repository contains source code written in [TRSE](https://lemonspawn.com/turbo-rascal-syntax-error-expected-but-begin/) by Nicolaas Groeneboom.

TimTris is a Tetris clone for TIM-011, school computer from Ex-Yugoslavia. Original Tetris is by Alexey Pajitnov.

Game uses graphics libraries written by me, and file libraries partially adapted from MESCC code by Miguel Garcia Lopez, both part of TRSE.

PTX Player is by S.V.Bulba, adapted to TIM-011 by Marko Šolajić. To compile it, you'll need [ZASM](https://k1.spdns.de/Develop/Projects/zasm/Distributions/).

"Tetris 2 Red Dawn" music is by Frantisek Fuka.

Many thanks to Marko for helping with various TIM stuff and Mićko for TIM-011 MAME emulator!

## Requirements

Working TIM-011 (there maybe 10 or less left in the world, so not a huge crowd) with AY sound card (needed if you wan to hear music).

Oooor...

MAME emulator, compiled from latest GitHub source, version 0.263 or later.

## Running

On real machine, after boot, type <r> <Return> to start it, and switch to lower case letters if needed.

On emulator, run it with:

```
mame tim011 -window -v -r 720x512 -switchres -exp ay -flop1 <image_name>.img
```

and do the same as on a real machine.

NOTICE: Computer needs around 30-40 seconds before anything shows on screen, so be patient!

## TODO

* File library still cannot save content, needs debugging/fixing (so high score cannot be saved)
* Adjustments to graphics (mostly cropping)
* Adding cursor keys as input (not yet fully supported by emulator)
* Game sometimes freezes/exits, but that one is so far observed only in emulator, not on real machine, needs more investigating

