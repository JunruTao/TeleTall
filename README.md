# My Game (Not yet titled)

This a repo for my Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). The Capstone Project is a chance to integrate what I've learned throughout this program. 
***
### Dependencies for Running Locally
* **cmake** >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* **make** >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* **SDL2** >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  * Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source.
* **gcc/g++** >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
      > This program was not yet tested building on Mac OS, be sure you follow the xcode instructions, and find the missing packages such as thread and SLD.h
  * Windows: recommend using [MinGW](http://www.mingw.org/)
      >  For windows users, make sure after download, using <ins>*MinGW Installation Manager*</ins> to install <ins>*mingw32-pthread-w32-dev*</ins> package or find c++ standard library of threading for MinGW from their websites!
***
### Basic Build Instructions

* **Linux:**
  1. Clone this repo.
  2. Make a build directory in the top level directory: `mkdir build && cd build`
  3. Compile: `cmake .. && make`
  4. Run it: `./${ProjectName}`

**<strong><em>! Important:</strong></em>** 
  * <ins>Because the environment that this project was developed was based in Window OS, therefore if you are building on Lunix System, Plese make sure you do the following things with the **source code**:</ins>

  | list |     Code Needed to Adjust              |    to  | Changed           |
  | :---  | ------------------------------------- | ---    | ----              |
  |  1.  | Change all the `#include <SDL2/SDL.h>`| &rarr; | `#include <SDL.h>`|
  | 2.*  | You might have to change `#include <pthread.h>` | &rarr; | `#include <thread>` |
  |  3.  | In<ins>`CMakeLists.txt`</ins> delete last few **-flags** in the last line | &rarr; | <s>`-lmingw32 -lSDL2main -lSDL2`</s> |
 


* **Windows:**
  1. Clone this repo.
  2. Make a build directory in the top level directory: `mkdir build` then `cd build`, two commands
  3. Compile: `cmake.exe -G "MinGW Makefiles" .. `
  4. Build(make): `mingw32-make` 
  5. Run it: `./${ProjectName}`
***
### Game Play and Usage

---JT 2020 - text = NULL---

***
### License

---JT 2020 - text = NULL---

***

### Release

---JT 2020 - text = NULL---

***