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
include_directories("../include/taglib/")


add_compile_options(
        -std=c++1y
            -Werror
            -Wall
            -Wextra
            -Wpedantic
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

