First of all: see http://www.turborascal.com

# TRSE
Turbo Rascal Syntax Error full repo 
- C++
- Qt

## Compiling
First, clone this repo to a TRSE directory.

Linux:
- apt-get install qt5-default qt5-qmake qtdeclarative5-dev mesa-common-dev
- qmake TRSE.pro
- make -j8 

On windows/macos, you need to download and install the qt libraries, msvc, xcode etc:
Windows:
- install MSVC 2019
- download the Qt framework from https://www.qt.io/download. Install the latest framework of Qt6 (desktop application).
Macos:
- install Xcode 
- download the Qt framework from https://www.qt.io/download. Install the latest framework of Qt6 (desktop application).

Select "Release", and under the qt project/build make sure you set the build directory to be **TRSE/Release**

## After first compile:
TRSE uses a couple of directories that needs to be linked with symlinks:
- Copy the directory "themes" in **TRSE/Publish/source/** to the **TRSE/Release** build directory 
- Make a symbolic link called "tutorials from your build directory to point to Publish/tutorials to access tutorial projects from the front page 
- Make a symbolic link called "units from your build directory to point to TRSE/Units to access the TRSE library 
- Make a symbolic link "project_templates" from your build directory to point to Publish/project_templates in order to access the "New Project" templates


# Source code information
A compiler UML diagram can be found here: https://github.com/leuat/TRSE/blob/master/uml/compiler.png

![Anurag's github stats](https://github-readme-stats.vercel.app/api?username=anuraghazra)
