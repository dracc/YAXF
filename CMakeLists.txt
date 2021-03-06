cmake_minimum_required(VERSION 3.8.0)
project(YAXF)

include(FindPkgConfig)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CXX g++)

set(sources config.cpp
    main.cpp
    mainwindow.cpp
    settingsmanager.cpp
    )

#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")

add_executable(${PROJECT_NAME} ${sources} ${data} icons.qrc)

target_compile_options(${PROJECT_NAME} PUBLIC -std=c++1y -Wall)


pkg_check_modules(LIBUSB REQUIRED libusb-1.0)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

include_directories(${LIBUSB_INCLUDE_DIRS})
message(SYSTEM ${LIBUSB_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}
    Qt5::Core Qt5::Widgets ${LIBUSB_LIBRARIES})

