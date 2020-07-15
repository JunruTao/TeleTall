cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS} -std=c++0x -pthread")####pThread

project(TELETALL_PROJECT)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)####pThread
set(THREADS_PREFER_PTHREAD_FLAG TRUE)####pThread

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_TTF REQUIRED)


file(GLOB_RECURSE TELETALL_PROJECT_SOURCES "source/*.cpp")
file(GLOB_RECURSE TELETALL_PROJECT_HEADERS "source/*.h")
set(TELETALL_INCLUDE_DIRS "")
foreach(_headerFile ${TELETALL_PROJECT_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND TELETALL_INCLUDE_DIRS ${_dir})
endforeach(_headerFile ${TELETALL_PROJECT_HEADERS})
list(REMOVE_DUPLICATES TELETALL_INCLUDE_DIRS)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR} source ui JUTA mingw_std_threads)
add_executable(teletall ${TELETALL_PROJECT_SOURCES})

find_package(Threads REQUIRED) ####pThread

target_include_directories(teletall PRIVATE ${TELETALL_INCLUDE_DIRS} Threads::Threads)####pThread

string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES )

TARGET_LINK_LIBRARIES(teletall ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} -lpthread -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf)

