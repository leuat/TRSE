First of all: see http://www.turborascal.com

# TRSE
Turbo Rascal Syntax Error full repo 
- C++
- Qt

## Compiling
- **IMPORTANT**: On windows, TRSE will ONLY COMPILE when using **MSVC 2017**. 2019 will fail due to some older libraries! Will be updated later.
- First, download the Qt framework from https://www.qt.io/download. Install the latest framework (desktop application).
- Clone this repo to a TRSE directory
  - On linux, you need to install a library that contains gl.h like mesa-common-dev (sudo apt-get install mesa-common-dev) etc
- Select "Release", and under the qt project/build make sure you set the build directory to be **TRSE/Release**
- Copy the directory "themes" in **TRSE/Publish/source/** to the **TRSE/Release** build directory 
- Make a symbolic link called "tutorials from your build directory to point to Publish/tutorials to access tutorial projects from the front page 
- Make a symbolic link "project_templates" from your build directory to point to Publish/project_templates in order to access the "New Project" templates
- Compile & run!

