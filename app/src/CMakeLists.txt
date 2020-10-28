cmake_minimum_required(VERSION 3.17)

set(SOURCES mainwindow.cpp
#        sound_tags.cpp
        musictablemodel.cpp
        tag_functions.cpp
        tableviewer.cpp
        searcher.cpp
        SoundPlayer.cpp
        loggingcategories.cpp
        settings.cpp
        app.cpp
        dialogpreferences.cpp
        settings.cpp
        dialoginfo.cpp
        medialibrary.cpp
        clickedlabel.cpp
        music.cpp
        playlist.cpp
        playerplaylist.cpp
        )

set(HEADERS mainwindow.h
#        sound_tags.h
        tag_functions.h
        musictablemodel.h
        tableviewer.h
        searcher.h
        SoundPlayer.h
        loggingcategories.h
        settings.h
        app.h
        dialogpreferences.h
        settings.h
        dialoginfo.h
        medialibrary.h
        clickedlabel.h
        music.h
        playlist.h
        playerplaylist.h
        )

set(UI mainwindow.ui dialogpreferences.ui dialoginfo.ui)

link_directories(../../build/3dparty/taglib/)


add_library(src ${SOURCES} ${HEADERS} ${UI})

target_link_libraries(src PRIVATE Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Multimedia
        tag
)
