#!/bin/bash
#export CMAKE_PREFIX_PATH=/c/Qt/5.11.1/mingw53_32
export CXX=g++

mkdir build
cd build
cmake .. -G "MSYS Makefiles"
make

strip YAXF.exe
