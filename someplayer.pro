#-------------------------------------------------
#
# Project created by QtCreator 2010-09-04T01:48:53
#
#-------------------------------------------------

QT       += core gui phonon sql

TARGET = someplayer
TEMPLATE = app


SOURCES += src/main.cpp\
        src/mainwindow.cpp \
		src/player/player.cpp \
        src/track.cpp \
    src/trackmetainformation.cpp \
    src/playlist.cpp \
    src/library.cpp \
    src/filestorage.cpp \
    src/dbstorage.cpp \
    src/mediascanner.cpp \
    src/tagresolver.cpp \
    src/playerform.cpp \
    src/libraryform.cpp \
    src/busywidget.cpp \
    src/trackrenderer.cpp

HEADERS  += src/mainwindow.h \
		src/player/player.h \
		src/track.h \
    src/trackmetainformation.h \
    src/playlist.h \
    src/someplayer.h \
    src/library.h \
    src/storage.h \
    src/filestorage.h \
    src/dbstorage.h \
    src/mediascanner.h \
    src/tagresolver.h \
    src/playerform.h \
    src/libraryform.h \
    src/busywidget.h \
    src/trackrenderer.h

FORMS    += src/ui/mainwindow.ui \
    src/ui/playerform.ui \
    src/ui/libraryform.ui \
    src/ui/busywidget.ui

CONFIG += mobility
MOBILITY = 

symbian {
    TARGET.UID3 = 0xebd21b9a
    # TARGET.CAPABILITY += 
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}

RESOURCES += \
    resources/resources.qrc

OTHER_FILES += \
    README \
    COPYING
