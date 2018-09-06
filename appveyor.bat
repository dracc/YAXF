rem Matrix-driven Appveyor CI script for YAXF, borrowed from https://github.com/xqemu/xqemu
rem Currently only does MSYS2 builds.
rem https://www.appveyor.com/docs/installed-software#mingw-msys-cygwin
rem Needs the following vars:
rem    MSYS2_ARCH:  x86_64 or i686
rem    MSYSTEM:  MINGW64 or MINGW32

bash -xlc "ls /c/Qt/5.11.1/mingw53_32"

rem Set the paths appropriately
PATH C:\msys64\%MSYSTEM%\bin;C:\msys64\usr\bin;%PATH%

rem Upgrade the MSYS2 platform
bash -lc "pacman --noconfirm --sync --refresh --refresh pacman"
bash -lc "pacman --noconfirm --sync --refresh --refresh --sysupgrade --sysupgrade"

rem Install required tools
bash -xlc "pacman --noconfirm -S --needed base-devel"

rem Install the relevant native dependencies
bash -xlc "pacman --noconfirm -S --needed git"
bash -xlc "pacman --noconfirm -S --needed mingw-w64-x86_64-cmake"
bash -xlc "pacman --noconfirm -S --needed mingw-w64-x86_64-make"
bash -xlc "pacman --noconfirm -S --needed mingw-w64-x86_64-libusb mingw-w64-x86_64-jasper"
bash -xlc "pacman --noconfirm -S --needed mingw-w64-x86_64-qt5-static"

rem Invoke subsequent bash in the build tree
cd %APPVEYOR_BUILD_FOLDER%
set CHERE_INVOKING=yes

rem Build/test scripting
bash -xlc "set pwd"
bash -xlc "env"
bash -xlc "./build.sh"
