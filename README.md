First of all: see http://www.turborascal.com

# TRSE
Turbo Rascal Syntax Error full repo 
- C++
- Qt

## Compiling

- First, download the Qt framework from https://www.qt.io/download. Install the latest framework (desktop application).
- Clone this repo to a TRSE directory
  - On linux, you need to install a library that contains gl.h like **like mesa-comhttps://www.facebook.com/mon-dev** (sudo apt-get install mesa-common-dev) etc
- Select "Release", and under the qt project/build make sure you set the build directory to be **TRSE/Release**
- Copy the directory "themes" in **TRSE/Publish/source/** to the **TRSE/Release** build directory 
- Make a symbolic link called "tutorials from your build directory to point to Publish/tutorials to access tutorial projects from the front page 
- Compile & run!

## To do
- Rewrite to newer C++ standard, remove all old cast-styles
- Rewrite compiler (parser, lexer and assembler)
