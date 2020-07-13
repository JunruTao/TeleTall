cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS} -std=c++0x")

project(TellTao_Project)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR} source)


add_executable(teletall source/main.cpp source/teletall.cpp source/telepad.cpp source/tallwindow.cpp source/teletall_control.cpp)
string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES )

TARGET_LINK_LIBRARIES(teletall ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf)

