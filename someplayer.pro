#-------------------------------------------------
#
# Project created by QtCreator 2010-09-04T01:48:53
#
#-------------------------------------------------

QT       += core gui phonon sql

TARGET = someplayer
TEMPLATE = app

INCLUDEPATH += src/taglib
INCLUDEPATH += src/taglib/toolkit
INCLUDEPATH += src/taglib/ape
INCLUDEPATH += src/taglib/asf
INCLUDEPATH += src/taglib/flac
INCLUDEPATH += src/taglib/mp4
INCLUDEPATH += src/taglib/mpc
INCLUDEPATH += src/taglib/mpeg
INCLUDEPATH += src/taglib/mpeg/id3v1
INCLUDEPATH += src/taglib/mpeg/id3v2
INCLUDEPATH += src/taglib/ogg
INCLUDEPATH += src/taglib/ogg/flac
INCLUDEPATH += src/taglib/ogg/speex
INCLUDEPATH += src/taglib/ogg/vorbis
INCLUDEPATH += src/taglib/riff
INCLUDEPATH += src/taglib/riff/aiff
INCLUDEPATH += src/taglib/riff/wav
INCLUDEPATH += src/taglib/trueaudio
INCLUDEPATH += src/taglib/wavpack

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
    src/trackrenderer.cpp \
    src/config.cpp \
    src/playlistdialog.cpp \
    src/taglib/tagunion.cpp \
    src/taglib/tag.cpp \
    src/taglib/fileref.cpp \
    src/taglib/audioproperties.cpp \
    src/taglib/ape/apetag.cpp \
    src/taglib/ape/apeitem.cpp \
    src/taglib/ape/apefooter.cpp \
    src/taglib/asf/asftag.cpp \
    src/taglib/asf/asfproperties.cpp \
    src/taglib/asf/asffile.cpp \
    src/taglib/asf/asfattribute.cpp \
    src/taglib/flac/flacproperties.cpp \
    src/taglib/flac/flacfile.cpp \
    src/taglib/mp4/mp4tag.cpp \
    src/taglib/mp4/mp4properties.cpp \
    src/taglib/mp4/mp4item.cpp \
    src/taglib/mp4/mp4file.cpp \
    src/taglib/mp4/mp4coverart.cpp \
    src/taglib/mp4/mp4atom.cpp \
    src/taglib/mpc/mpcproperties.cpp \
    src/taglib/mpc/mpcfile.cpp \
    src/taglib/mpeg/xingheader.cpp \
    src/taglib/mpeg/mpegproperties.cpp \
    src/taglib/mpeg/mpegheader.cpp \
    src/taglib/mpeg/mpegfile.cpp \
    src/taglib/mpeg/id3v1/id3v1tag.cpp \
    src/taglib/mpeg/id3v1/id3v1genres.cpp \
    src/taglib/mpeg/id3v2/id3v2tag.cpp \
    src/taglib/mpeg/id3v2/id3v2synchdata.cpp \
    src/taglib/mpeg/id3v2/id3v2header.cpp \
    src/taglib/mpeg/id3v2/id3v2framefactory.cpp \
    src/taglib/mpeg/id3v2/id3v2frame.cpp \
    src/taglib/mpeg/id3v2/id3v2footer.cpp \
    src/taglib/mpeg/id3v2/id3v2extendedheader.cpp \
    src/taglib/mpeg/id3v2/frames/urllinkframe.cpp \
    src/taglib/mpeg/id3v2/frames/unsynchronizedlyricsframe.cpp \
    src/taglib/mpeg/id3v2/frames/unknownframe.cpp \
    src/taglib/mpeg/id3v2/frames/uniquefileidentifierframe.cpp \
    src/taglib/mpeg/id3v2/frames/textidentificationframe.cpp \
    src/taglib/mpeg/id3v2/frames/relativevolumeframe.cpp \
    src/taglib/mpeg/id3v2/frames/privateframe.cpp \
    src/taglib/mpeg/id3v2/frames/popularimeterframe.cpp \
    src/taglib/mpeg/id3v2/frames/generalencapsulatedobjectframe.cpp \
    src/taglib/mpeg/id3v2/frames/commentsframe.cpp \
    src/taglib/mpeg/id3v2/frames/attachedpictureframe.cpp \
    src/taglib/ogg/xiphcomment.cpp \
    src/taglib/ogg/oggpageheader.cpp \
    src/taglib/ogg/oggpage.cpp \
    src/taglib/ogg/oggfile.cpp \
    src/taglib/ogg/flac/oggflacfile.cpp \
    src/taglib/ogg/speex/speexproperties.cpp \
    src/taglib/ogg/speex/speexfile.cpp \
    src/taglib/ogg/vorbis/vorbisproperties.cpp \
    src/taglib/ogg/vorbis/vorbisfile.cpp \
    src/taglib/riff/rifffile.cpp \
    src/taglib/riff/aiff/aiffproperties.cpp \
    src/taglib/riff/aiff/aifffile.cpp \
    src/taglib/riff/wav/wavproperties.cpp \
    src/taglib/riff/wav/wavfile.cpp \
    src/taglib/toolkit/unicode.cpp \
    src/taglib/toolkit/tstringlist.cpp \
    src/taglib/toolkit/tstring.cpp \
    src/taglib/toolkit/tfile.cpp \
    src/taglib/toolkit/tdebug.cpp \
    src/taglib/toolkit/tbytevectorlist.cpp \
    src/taglib/toolkit/tbytevector.cpp \
    src/taglib/trueaudio/trueaudioproperties.cpp \
    src/taglib/trueaudio/trueaudiofile.cpp \
    src/taglib/wavpack/wavpackproperties.cpp \
    src/taglib/wavpack/wavpackfile.cpp \
    src/edittagsdialog.cpp \
    src/timerdialog.cpp \
    src/equalizerdialog.cpp \
    src/saveplaylistdialog.cpp \
    src/settingsdialog.cpp

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
    src/trackrenderer.h \
    src/config.h \
    src/playlistdialog.h \
    src/taglib/taglib_export.h \
    src/taglib/tagunion.h \
    src/taglib/tag.h \
    src/taglib/fileref.h \
    src/taglib/audioproperties.h \
    src/taglib/ape/apetag.h \
    src/taglib/ape/apeitem.h \
    src/taglib/ape/apefooter.h \
    src/taglib/asf/asftag.h \
    src/taglib/asf/asfproperties.h \
    src/taglib/asf/asffile.h \
    src/taglib/asf/asfattribute.h \
    src/taglib/flac/flacproperties.h \
    src/taglib/flac/flacfile.h \
    src/taglib/mp4/mp4tag.h \
    src/taglib/mp4/mp4properties.h \
    src/taglib/mp4/mp4item.h \
    src/taglib/mp4/mp4file.h \
    src/taglib/mp4/mp4coverart.h \
    src/taglib/mp4/mp4atom.h \
    src/taglib/mpc/mpcproperties.h \
    src/taglib/mpc/mpcfile.h \
    src/taglib/mpeg/xingheader.h \
    src/taglib/mpeg/mpegproperties.h \
    src/taglib/mpeg/mpegheader.h \
    src/taglib/mpeg/mpegfile.h \
    src/taglib/mpeg/id3v1/id3v1tag.h \
    src/taglib/mpeg/id3v1/id3v1genres.h \
    src/taglib/mpeg/id3v2/id3v2tag.h \
    src/taglib/mpeg/id3v2/id3v2synchdata.h \
    src/taglib/mpeg/id3v2/id3v2header.h \
    src/taglib/mpeg/id3v2/id3v2framefactory.h \
    src/taglib/mpeg/id3v2/id3v2frame.h \
    src/taglib/mpeg/id3v2/id3v2footer.h \
    src/taglib/mpeg/id3v2/id3v2extendedheader.h \
    src/taglib/mpeg/id3v2/frames/urllinkframe.h \
    src/taglib/mpeg/id3v2/frames/unsynchronizedlyricsframe.h \
    src/taglib/mpeg/id3v2/frames/unknownframe.h \
    src/taglib/mpeg/id3v2/frames/uniquefileidentifierframe.h \
    src/taglib/mpeg/id3v2/frames/textidentificationframe.h \
    src/taglib/mpeg/id3v2/frames/relativevolumeframe.h \
    src/taglib/mpeg/id3v2/frames/privateframe.h \
    src/taglib/mpeg/id3v2/frames/popularimeterframe.h \
    src/taglib/mpeg/id3v2/frames/generalencapsulatedobjectframe.h \
    src/taglib/mpeg/id3v2/frames/commentsframe.h \
    src/taglib/mpeg/id3v2/frames/attachedpictureframe.h \
    src/taglib/ogg/xiphcomment.h \
    src/taglib/ogg/oggpageheader.h \
    src/taglib/ogg/oggpage.h \
    src/taglib/ogg/oggfile.h \
    src/taglib/ogg/flac/oggflacfile.h \
    src/taglib/ogg/speex/speexproperties.h \
    src/taglib/ogg/speex/speexfile.h \
    src/taglib/ogg/vorbis/vorbisproperties.h \
    src/taglib/ogg/vorbis/vorbisfile.h \
    src/taglib/riff/rifffile.h \
    src/taglib/riff/aiff/aiffproperties.h \
    src/taglib/riff/aiff/aifffile.h \
    src/taglib/riff/wav/wavproperties.h \
    src/taglib/riff/wav/wavfile.h \
    src/taglib/toolkit/unicode.h \
    src/taglib/toolkit/tstringlist.h \
    src/taglib/toolkit/tstring.h \
    src/taglib/toolkit/tmap.h \
    src/taglib/toolkit/tlist.h \
    src/taglib/toolkit/tfile.h \
    src/taglib/toolkit/tdebug.h \
    src/taglib/toolkit/tbytevectorlist.h \
    src/taglib/toolkit/tbytevector.h \
    src/taglib/toolkit/taglib.h \
    src/taglib/trueaudio/trueaudioproperties.h \
    src/taglib/trueaudio/trueaudiofile.h \
    src/taglib/wavpack/wavpackproperties.h \
    src/taglib/wavpack/wavpackfile.h \
    src/edittagsdialog.h \
    src/timerdialog.h \
    src/equalizerdialog.h \
    src/saveplaylistdialog.h \
    src/settingsdialog.h \
    src/abstractitemrenderer.h

FORMS    += src/ui/mainwindow.ui \
    src/ui/playerform.ui \
    src/ui/libraryform.ui \
    src/ui/busywidget.ui \
    src/ui/playlistdialog.ui \
    src/ui/edittagsdialog.ui \
    src/ui/timerdialog.ui \
    src/ui/equalizerdialog.ui \
    src/ui/saveplaylistdialog.ui \
    src/ui/settingsdialog.ui

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
    COPYING \
    resources/someplayer.desktop \
    src/taglib/taglib_config.h.in \
    src/taglib/ape/ape-tag-format.txt \
    src/taglib/mpeg/id3v2/id3v2.4.0-structure.txt \
    src/taglib/mpeg/id3v2/id3v2.4.0-frames.txt \
    src/taglib/mpeg/id3v2/id3v2.3.0.txt \
    src/taglib/mpeg/id3v2/id3v2.2.0.txt \
    src/taglib/toolkit/tmap.tcc \
    src/taglib/toolkit/tlist.tcc \
    AUTHORS.TagLib \
    COPYING.LGPL
