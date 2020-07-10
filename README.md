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

None


***

## Production Logs:
* #### DAY 1 { <ins>7/9/2020</ins> } :
First time trying to build the project with cmake and MinGW-make compiler, including and linking the SLD headers and libraries. There was some interesting issues with building SLD on windows. 
 |Findings and Solutions |
 |:---|
 | <sub> **[ General: ]** |
 | **1.** CMake: The first is that the -flags in the CMakeLists.txt `-lmingw32 -lSDL2main -lSDL2` must be included in the last line;|
  |**2.** Includes: The `SDL2/SDL.h` should be in the include instead of `SDL.h`.|
 | **3.** CMake: Some Extra things I've learnt, that if you have headers in a separete directory, you have to put that into CMakeLists.txt argument: `include_directories( ... header)`. And apparently you don't have to put header files `.h` into `add_executable(..)` argument list.|
 | <sub> **[ Regarding SDL Code: ]**  |
 | **4.** Building the program I found another problem with the example code is that the <s>`int main()`</s> fuction must include extra argument input parameters: which is: `int main`**`( int argc, char* args[] )`**. My assumption is that the SDL.h's implementation on windows is using the library `windows.h` so that the main window is actually : `int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE  hPrevInstance, PWSTR lpCmdLine, int nCmdShow)` there is a quite good [explaination](https://stackoverflow.com/questions/13871617/winmain-and-main-in-c-extended) I found.|
 | **5.** The first program that I was trying to test the SDL context is loading a bitmap to display on the screen for few seconds, the source code reference is from [SDL wiki](http://lazyfoo.net/tutorials/SDL/02_getting_an_image_on_the_screen/index.php). And I found somehow it not printing anything to the console and just quit. I assume might be the IDE and it seems not running as a concole app on windows. the solution I found is [here](https://stackoverflow.com/questions/31571897/window-closes-too-fast). |
 | &rarr; <sup>**5. 1**</sup> The first problem: if using `SDL_Delay(1000);`, In the Initiation function `init()` => `SDL_Init(SDL_INIT_VIDEO | ` <ins>**`SDL_INIT_TIMER`** </ins> `)` timer's init flag must added. |
 | &rarr; <sup>**5. 2**</sup> Secondly <s>`if (!ContextInit())`</s> then <s>{ `if(!LoadMedia){..}else{..}` }</s> this logic won't work, somehow it will be skipped by the main and quit the program real fast if there something when wrong in the initiation. the correct way is "run if returns true then else" like this: `if ( ContextInit() ){ if(LoadMedia){..<main features>..}else{..} }else{..}` will do just fine. |
 | &rarr; <sup>**5. 3**</sup> The last one is a trick one as well, which is the media loading. The main reason it quits: <ins>*The Bitmap file must be placed in the **same directory** where the `.exe` or your IDE's project is located*</ins>. Reference solution can be found [here](https://stackoverflow.com/questions/38012690/cant-load-bmp-file-with-sdl-loadbmp-in-codeblocks). And then everything is running just fine.

 * #### DAY 2 { <ins>M/D/2020</ins> } :