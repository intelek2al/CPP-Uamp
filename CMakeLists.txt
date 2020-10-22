#cmake_minimum_required(VERSION 3.15)

##project(utag VERSION 1.0 LANGUAGES CXX)

#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_CXX_STANDARD_REQUIRED YES)

#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)

##set(TAGLIB_DIR "D:/Projects/C++/CPP-Utag/3dparty/taglib")

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

#find_library(TAGLIB taglib)

#include_directories(./src/)
#include_directories(../cmake-build-debug/app/src/src_autogen/include)
#include_directories(../3dparty/taglib/)
#include_directories(../3dparty/taglib/taglib/)
#include_directories(../3dparty/taglib/taglib/mpeg/)
#include_directories(../3dparty/taglib/taglib/mpeg/id3v2/)
#include_directories(../3dparty/taglib/taglib/frames/)
#include_directories(../3dparty/taglib/taglib/toolkit)
#include_directories(../build/3dparty/taglib/)
#include_directories(../cmake-build-debug/3dparty/taglib)

#find_library(TAGLIB_LIB taglib)

#if(CMAKE_VERSION VERSION_LESS "3.7.0")
#    set(CMAKE_INCLUDE_CURRENT_DIR ON)
#endif()

#find_package(Qt5Core REQUIRED)
#find_package(Qt5Widgets REQUIRED)
#find_package(Qt5Quick REQUIRED)

#find_package(Qt5Multimedia REQUIRED)

#find_package(Qt5MultimediaWidgets REQUIRED )

#set(Qt5_DIR 'D:/Qt/5.15.1/msvc2019_64/lib/cmake')

##set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")

#SET(QT_USE_QTMULTIMEDIA TRUE)
#SET(QT_USE_QTMULTIMEDIAWIDGETS TRUE)

#set(SOURCES main.cpp src/mainwindow.cpp
#        src/musictablemodel.cpp
#        src/tag_functions.cpp
#        src/tableviewer.cpp
#        src/SoundPlayer.cpp)

#set(HEADERS src/mainwindow.h
#        src/tag_functions.h
#        src/musictablemodel.h
#        src/tableviewer.h
#        src/SoundPlayer.h)

#set(UI src/mainwindow.ui)

#add_executable(utag ${SOURCES} ${HEADERS} ${UI})


#set(CMAKE_INCLUDE_CURRENT_DIR ON)

##include_directories(../3dparty/taglib/taglib/)

#set(TAGLIB_VERSION 1.8)
#set(TAGLIB_INCLUDE_DIRS "${CMAKE_BINARY_DIR}/3dparty/taglib/taglib/;${CMAKE_BINARY_DIR}/3dparty/taglib/;${CMAKE_CURRENT_SOURCE_DIR}/../3dparty/taglib/taglib")
#set(TAGLIB_LIBRARY_DIRS "")
#set(TAGLIB_LIBRARIES tag)
#message("${CMAKE_CURRENT_SOURCE_DIR}/../3dparty/taglib")
##add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../3dparty/taglib)

#link_directories(${TAGLIB_LIBRARIES})

#include_directories(${TAGLIB_INCLUDE_DIRS})

#target_link_libraries(utag "${TAGLIB_LIBRARIES}")

#target_link_libraries(utag Qt5::Widgets)
#target_link_libraries(utag Qt5::Core)
#target_link_libraries(utag Qt5::Multimedia)
#target_link_libraries(utag Qt5::MultimediaWidgets)

#include_directories(../3dparty/taglib/taglib/)
#include_directories(../3dparty/taglib/taglib/mpeg/)
#include_directories(../3dparty/taglib/taglib/mpeg/id3v2/)
#include_directories(../3dparty/taglib/taglib/frames/)
#include_directories(../3dparty/taglib/taglib/toolkit)

##include_directories(../build/3dparty/taglib/)
##include_directories(../cmake-build-debug/3dparty/taglib/)

#include_directories(${Qt5_DIR})

#include_directories(${Qt5Multimedia_INCLUDES})

#include_directories(src)

#add_compile_options(
#        -std=c++17
##        -Wdev
#         -Werror
#         -Wall
#         -Wextra
#         -Wpedantic
##        -g -fsanitize=address;
##        -Wno-unused-parameter
#)


##target_link_libraries(utag tag)

#target_link_libraries(utag Qt5::Core)
#target_link_libraries(utag Qt5::Widgets)
#target_link_libraries(utag Qt5::Quick)

##QT5_USE_MODULES (utag Multimedia MultimediaWidgets)
#target_link_libraries(utag ${Qt5Multimedia_LIBRARIES})
##target_link_libraries(utag Qt5MultimediaWidgets)

#add_subdirectory(src)




##set(Qt5Core_DIR "~/Qt/5.15.1/clang_64/lib/cmake/Qt5Core/")

##find_package(QT NAMES Qt6 Qt5 COMPONENTS Qt5Core REQUIRED)
##find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Qt5Core REQUIRED)

##find_package(Qt5 COMPONENTS Qt5Core REQUIRED)
##find_package(Qt5 COMPONENTS Qt5Widgets REQUIRED)
##find_package(Qt5 COMPONENTS Qt5Quick REQUIRED)


##set(Qt5Core_DIR "~/.brew/Cellar/qt/5.15.1/lib/cmake/Qt5Core")
##set(Qt5_DIR "~/.brew/Cellar/qt/5.15.1/lib/cmake/Qt5")

##set(CMAKE_PREFIX_PATH "~/.brew/Cellar/qt/5.15.1/lib/cmake/Qt5Core")
##target_include_directories(utag INTERFACE "../3dparty/taglib/taglib/")


cmake_minimum_required(VERSION 3.15)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_library(TAGLIB tag)

include_directories(./src/)
#include_directories("../build/app/src/src_autogen/include")#
include_directories("../cmake-build-debug/app/src/src_autogen/include")
include_directories(../3dparty/taglib/)
include_directories(../3dparty/taglib/taglib/)
include_directories(../3dparty/taglib/taglib/mpeg/)
include_directories(../3dparty/taglib/taglib/mpeg/id3v2/)
include_directories(../3dparty/taglib/taglib/frames/)
include_directories(../3dparty/taglib/taglib/toolkit)
include_directories(../build/3dparty/taglib/)
# include_directories("../include/taglib/")#


add_compile_options(
    -std=c++1y
#    -Werror
#    -Wall
#    -Wextra
#    -Wpedantic
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Core Gui Multimedia MultimediaWidgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Core Gui Multimedia MultimediaWidgets REQUIRED)

# link_directories(../lib)

add_subdirectory(src)

qt5_add_resources(RESOURCES resources/myresource.qrc)

add_executable(${CMAKE_PROJECT_NAME} main.cpp ${RESOURCES})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Multimedia
        src
        tag# tag
)

