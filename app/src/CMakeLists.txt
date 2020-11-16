cmake_minimum_required(VERSION 3.17)

set(SOURCES mainwindow.cpp
        tag_functions.cpp
        tableviewer.cpp
        searcher.cpp
        soundPlayer.cpp
        loggingcategories.cpp
        settings.cpp
        app.cpp
        dialogpreferences.cpp
        settings.cpp
        dialoginfo.cpp
#        medialibrary.cpp
        clickedlabel.cpp
        music.cpp
        playlist.cpp
        playerplaylist.cpp
        starrating.cpp
        stardelegate.cpp
        stareditor.cpp
#        sqlbase.cpp
        nextup.cpp
        nextupwidget.cpp
        singletonbase.cpp
        songmodel.cpp
        songquerymodel.cpp
        )

set(HEADERS mainwindow.h
        tag_functions.h
        tableviewer.h
        searcher.h
        soundPlayer.h
        loggingcategories.h
        settings.h
        app.h
        dialogpreferences.h
        settings.h
        dialoginfo.h
#        medialibrary.h
        clickedlabel.h
        music.h
        playlist.h
        playerplaylist.h
        starrating.h
        stardelegate.h
        stareditor.h
#        sqlbase.h
        nextup.h
        nextupwidget.h
        playerqss.h
        singletonbase.h
        songmodel.h
        songquerymodel.h
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
