# TRSE
Turbo Rascal Syntax Error full repo 
- C++
- Qt

## Compiling

- First, download the Qt framework from https://www.qt.io/download. Install the latest framework (desktop application).
- Clone this repo to a TRSE directory
  - On linux, you need to install a library that contains gl.h (**like mesa-common-dev** (sudo apt-get install mesa-common-dev) etc
- Select "Release", and under the qt project/build make sure you set the build directory to be **TRSE/Release**
- Copy the two directories in **TRSE/Publish/source/** to the **TRSE/Release** build directory (these directories contain run-time settings files)
- Compile & run!

## To do
- Rewrite to newer C++ standard, remove all old cast-styles
- Rewrite compiler (parser, lexer and assembler)
