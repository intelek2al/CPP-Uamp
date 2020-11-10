cmake_minimum_required(VERSION 3.17)

set(SOURCES mainwindow.cpp
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
        starrating.cpp
        stardelegate.cpp
        stareditor.cpp
        sqlbase.cpp
        nextup.cpp
        nextupwidget.cpp
        )

set(HEADERS mainwindow.h
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
        starrating.h
        stardelegate.h
        stareditor.h
        sqlbase.h
        nextup.h
        nextupwidget.h
        playerqss.h
        )

set(UI mainwindow.ui dialogpreferences.ui dialoginfo.ui nextup.ui nextupwidget.ui playerqss.h)

link_directories(../../build/3dparty/taglib/)


add_library(src ${SOURCES} ${HEADERS} ${UI})

target_link_libraries(src PRIVATE Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Multimedia
        Qt${QT_VERSION_MAJOR}::Sql
        tag
)
