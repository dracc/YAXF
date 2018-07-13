# YAXF
A simple yet powerful tool for configuring your XQEMU session.

## Requirements
Qt >= 5.8  
libusb = 1.0

To install these in Debian/Ubuntu/etc. run the following command
in a terminal window:  
```apt-get install qt5-default libusb-1.0-0-dev```

## How to build
Either open the project in Qt Creator and build it through there
or use **qmake**;
```
mkdir build
cd build
qmake ..
make -j
```
