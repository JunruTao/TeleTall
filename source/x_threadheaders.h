#ifndef X_THREADHEADERS_H
#define X_THREADHEADERS_H

/////////////////////////////TELETALL 2020///////////////////////////
////IF Building on Linux, please change this to _WINUSER_ to 0///////
/////////////////////////////////////////////////////////////////////
#define _WINUSER_ 1

#if _WINUSER_ == 1
#define _WIN32_WINNT  0x0501

#include "mingw.thread.h"
#include "mingw.future.h"
#include "mingw.condition_variable.h"
#include "mingw.mutex.h"
#else
#include <thread>
#include <mutex>
#include <future>
#include <condition_variable>
#endif


#endif