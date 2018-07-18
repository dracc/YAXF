# YAXF
A simple yet powerful tool for configuring your XQEMU session.

## Requirements
Qt >= 5.8  
libusb >= 1.0.20
cmake >= 3.8

To install these in Debian/Ubuntu/etc. run the following command
in a terminal window:  
```apt-get install build-essential cmake qt5-default libusb-1.0-0-dev```

## How to build
Either open the project in Qt Creator and build it through there
or use **cmake**;
```
mkdir build
cd build
cmake ..
make -j
```
