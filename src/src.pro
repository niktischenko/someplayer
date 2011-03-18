#-------------------------------------------------
#
# Project created by QtCreator 2010-09-04T01:48:53
#
#-------------------------------------------------

QT       += core gui phonon sql dbus webkit

TARGET = someplayer
TEMPLATE = app

INCLUDEPATH += taglib
INCLUDEPATH += taglib/toolkit
INCLUDEPATH += taglib/ape
INCLUDEPATH += taglib/asf
INCLUDEPATH += taglib/flac
INCLUDEPATH += taglib/mp4
INCLUDEPATH += taglib/mpc
INCLUDEPATH += taglib/mpeg
INCLUDEPATH += taglib/mpeg/id3v1
INCLUDEPATH += taglib/mpeg/id3v2
INCLUDEPATH += taglib/ogg
INCLUDEPATH += taglib/ogg/flac
INCLUDEPATH += taglib/ogg/speex
INCLUDEPATH += taglib/ogg/vorbis
INCLUDEPATH += taglib/riff
INCLUDEPATH += taglib/riff/aiff
INCLUDEPATH += taglib/riff/wav
INCLUDEPATH += taglib/trueaudio
INCLUDEPATH += taglib/wavpack

SOURCES += main.cpp\
        mainwindow.cpp \
		player/player.cpp \
        track.cpp \
    trackmetainformation.cpp \
    playlist.cpp \
    library.cpp \
    filestorage.cpp \
    dbstorage.cpp \
    mediascanner.cpp \
    tagresolver.cpp \
    playerform.cpp \
    libraryform.cpp \
    busywidget.cpp \
    trackrenderer.cpp \
    config.cpp \
    playlistdialog.cpp \
    taglib/tagunion.cpp \
    taglib/tag.cpp \
    taglib/fileref.cpp \
    taglib/audioproperties.cpp \
    taglib/ape/apetag.cpp \
    taglib/ape/apeitem.cpp \
    taglib/ape/apefooter.cpp \
    taglib/asf/asftag.cpp \
    taglib/asf/asfproperties.cpp \
    taglib/asf/asffile.cpp \
    taglib/asf/asfattribute.cpp \
    taglib/flac/flacproperties.cpp \
    taglib/flac/flacfile.cpp \
    taglib/mp4/mp4tag.cpp \
    taglib/mp4/mp4properties.cpp \
    taglib/mp4/mp4item.cpp \
    taglib/mp4/mp4file.cpp \
    taglib/mp4/mp4coverart.cpp \
    taglib/mp4/mp4atom.cpp \
    taglib/mpc/mpcproperties.cpp \
    taglib/mpc/mpcfile.cpp \
    taglib/mpeg/xingheader.cpp \
    taglib/mpeg/mpegproperties.cpp \
    taglib/mpeg/mpegheader.cpp \
    taglib/mpeg/mpegfile.cpp \
    taglib/mpeg/id3v1/id3v1tag.cpp \
    taglib/mpeg/id3v1/id3v1genres.cpp \
    taglib/mpeg/id3v2/id3v2tag.cpp \
    taglib/mpeg/id3v2/id3v2synchdata.cpp \
    taglib/mpeg/id3v2/id3v2header.cpp \
    taglib/mpeg/id3v2/id3v2framefactory.cpp \
    taglib/mpeg/id3v2/id3v2frame.cpp \
    taglib/mpeg/id3v2/id3v2footer.cpp \
    taglib/mpeg/id3v2/id3v2extendedheader.cpp \
    taglib/mpeg/id3v2/frames/urllinkframe.cpp \
    taglib/mpeg/id3v2/frames/unsynchronizedlyricsframe.cpp \
    taglib/mpeg/id3v2/frames/unknownframe.cpp \
    taglib/mpeg/id3v2/frames/uniquefileidentifierframe.cpp \
    taglib/mpeg/id3v2/frames/textidentificationframe.cpp \
    taglib/mpeg/id3v2/frames/relativevolumeframe.cpp \
    taglib/mpeg/id3v2/frames/privateframe.cpp \
    taglib/mpeg/id3v2/frames/popularimeterframe.cpp \
    taglib/mpeg/id3v2/frames/generalencapsulatedobjectframe.cpp \
    taglib/mpeg/id3v2/frames/commentsframe.cpp \
    taglib/mpeg/id3v2/frames/attachedpictureframe.cpp \
    taglib/ogg/xiphcomment.cpp \
    taglib/ogg/oggpageheader.cpp \
    taglib/ogg/oggpage.cpp \
    taglib/ogg/oggfile.cpp \
    taglib/ogg/flac/oggflacfile.cpp \
    taglib/ogg/speex/speexproperties.cpp \
    taglib/ogg/speex/speexfile.cpp \
    taglib/ogg/vorbis/vorbisproperties.cpp \
    taglib/ogg/vorbis/vorbisfile.cpp \
    taglib/riff/rifffile.cpp \
    taglib/riff/aiff/aiffproperties.cpp \
    taglib/riff/aiff/aifffile.cpp \
    taglib/riff/wav/wavproperties.cpp \
    taglib/riff/wav/wavfile.cpp \
    taglib/toolkit/unicode.cpp \
    taglib/toolkit/tstringlist.cpp \
    taglib/toolkit/tstring.cpp \
    taglib/toolkit/tfile.cpp \
    taglib/toolkit/tdebug.cpp \
    taglib/toolkit/tbytevectorlist.cpp \
    taglib/toolkit/tbytevector.cpp \
    taglib/trueaudio/trueaudioproperties.cpp \
    taglib/trueaudio/trueaudiofile.cpp \
    taglib/wavpack/wavpackproperties.cpp \
    taglib/wavpack/wavpackfile.cpp \
    edittagsdialog.cpp \
    timerdialog.cpp \
    equalizerdialog.cpp \
    saveplaylistdialog.cpp \
    dbusadaptor.cpp \
    toolswidget.cpp \
    managelibraryform.cpp \
    directoryview.cpp \
    coverfinder.cpp \
    clickablelabel.cpp \
    settingsform.cpp \
    dbusclient.cpp \
    fmtxsettingsdialog.cpp \
    aboutform.cpp \
    playlistsortform.cpp

HEADERS  += mainwindow.h \
		player/player.h \
		track.h \
    trackmetainformation.h \
    playlist.h \
    someplayer.h \
    library.h \
    storage.h \
    filestorage.h \
    dbstorage.h \
    mediascanner.h \
    tagresolver.h \
    playerform.h \
    libraryform.h \
    busywidget.h \
    trackrenderer.h \
    config.h \
    playlistdialog.h \
    taglib/taglib_export.h \
    taglib/tagunion.h \
    taglib/tag.h \
    taglib/fileref.h \
    taglib/audioproperties.h \
    taglib/ape/apetag.h \
    taglib/ape/apeitem.h \
    taglib/ape/apefooter.h \
    taglib/asf/asftag.h \
    taglib/asf/asfproperties.h \
    taglib/asf/asffile.h \
    taglib/asf/asfattribute.h \
    taglib/flac/flacproperties.h \
    taglib/flac/flacfile.h \
    taglib/mp4/mp4tag.h \
    taglib/mp4/mp4properties.h \
    taglib/mp4/mp4item.h \
    taglib/mp4/mp4file.h \
    taglib/mp4/mp4coverart.h \
    taglib/mp4/mp4atom.h \
    taglib/mpc/mpcproperties.h \
    taglib/mpc/mpcfile.h \
    taglib/mpeg/xingheader.h \
    taglib/mpeg/mpegproperties.h \
    taglib/mpeg/mpegheader.h \
    taglib/mpeg/mpegfile.h \
    taglib/mpeg/id3v1/id3v1tag.h \
    taglib/mpeg/id3v1/id3v1genres.h \
    taglib/mpeg/id3v2/id3v2tag.h \
    taglib/mpeg/id3v2/id3v2synchdata.h \
    taglib/mpeg/id3v2/id3v2header.h \
    taglib/mpeg/id3v2/id3v2framefactory.h \
    taglib/mpeg/id3v2/id3v2frame.h \
    taglib/mpeg/id3v2/id3v2footer.h \
    taglib/mpeg/id3v2/id3v2extendedheader.h \
    taglib/mpeg/id3v2/frames/urllinkframe.h \
    taglib/mpeg/id3v2/frames/unsynchronizedlyricsframe.h \
    taglib/mpeg/id3v2/frames/unknownframe.h \
    taglib/mpeg/id3v2/frames/uniquefileidentifierframe.h \
    taglib/mpeg/id3v2/frames/textidentificationframe.h \
    taglib/mpeg/id3v2/frames/relativevolumeframe.h \
    taglib/mpeg/id3v2/frames/privateframe.h \
    taglib/mpeg/id3v2/frames/popularimeterframe.h \
    taglib/mpeg/id3v2/frames/generalencapsulatedobjectframe.h \
    taglib/mpeg/id3v2/frames/commentsframe.h \
    taglib/mpeg/id3v2/frames/attachedpictureframe.h \
    taglib/ogg/xiphcomment.h \
    taglib/ogg/oggpageheader.h \
    taglib/ogg/oggpage.h \
    taglib/ogg/oggfile.h \
    taglib/ogg/flac/oggflacfile.h \
    taglib/ogg/speex/speexproperties.h \
    taglib/ogg/speex/speexfile.h \
    taglib/ogg/vorbis/vorbisproperties.h \
    taglib/ogg/vorbis/vorbisfile.h \
    taglib/riff/rifffile.h \
    taglib/riff/aiff/aiffproperties.h \
    taglib/riff/aiff/aifffile.h \
    taglib/riff/wav/wavproperties.h \
    taglib/riff/wav/wavfile.h \
    taglib/toolkit/unicode.h \
    taglib/toolkit/tstringlist.h \
    taglib/toolkit/tstring.h \
    taglib/toolkit/tmap.h \
    taglib/toolkit/tlist.h \
    taglib/toolkit/tfile.h \
    taglib/toolkit/tdebug.h \
    taglib/toolkit/tbytevectorlist.h \
    taglib/toolkit/tbytevector.h \
    taglib/toolkit/taglib.h \
    taglib/trueaudio/trueaudioproperties.h \
    taglib/trueaudio/trueaudiofile.h \
    taglib/wavpack/wavpackproperties.h \
    taglib/wavpack/wavpackfile.h \
    edittagsdialog.h \
    timerdialog.h \
    equalizerdialog.h \
    saveplaylistdialog.h \
    abstractitemrenderer.h \
    dbusadaptor.h \
    toolswidget.h \
    managelibraryform.h \
    directoryview.h \
    coverfinder.h \
    clickablelabel.h \
    settingsform.h \
    dbusclient.h \
    fmtxsettingsdialog.h \
    aboutform.h \
    playlistsortform.h

FORMS    += ui/mainwindow.ui \
    ui/playerform.ui \
    ui/libraryform.ui \
    ui/busywidget.ui \
    ui/playlistdialog.ui \
    ui/edittagsdialog.ui \
    ui/timerdialog.ui \
    ui/equalizerdialog.ui \
    ui/saveplaylistdialog.ui \
    ui/toolswidget.ui \
    ui/managelibraryform.ui \
    ui/directoryview.ui \
    ui/settingsform.ui \
    ui/fmtxsettingsdialog.ui \
    ui/aboutform.ui \
    ui/playlistsortform.ui

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
    taglib/taglib_config.h.in \
    taglib/ape/ape-tag-format.txt \
    taglib/mpeg/id3v2/id3v2.4.0-structure.txt \
    taglib/mpeg/id3v2/id3v2.4.0-frames.txt \
    taglib/mpeg/id3v2/id3v2.3.0.txt \
    taglib/mpeg/id3v2/id3v2.2.0.txt \
    taglib/toolkit/tmap.tcc \
    taglib/toolkit/tlist.tcc \
    AUTHORS.TagLib \
    COPYING.LGPL

TRANSLATIONS	=  resources/someplayer_ru.ts \
	resources/someplayer_zh_CN.ts \
	resources/someplayer_zh_TW.ts \
	resources/someplayer_it.ts


unix {
	#VARIABLES
	isEmpty(PREFIX) {
		PREFIX = /usr
      	}
	BINDIR = $$PREFIX/bin
	DATADIR =$$PREFIX/share
	
	DEFINES += DATADIR=\\\"$$DATADIR\\\" PKGDATADIR=\\\"$$PKGDATADIR\\\"
	
	#MAKE INSTALL
	
	INSTALLS += target desktop service iconxpm icon26 icon48 icon64
	
	target.path =$$BINDIR
	
	desktop.path = $$DATADIR/applications/hildon
	desktop.files += $${TARGET}.desktop
	
	service.path = $$DATADIR/dbus-1/services
	service.files += $${TARGET}.service
	
	icon64.path = $$DATADIR/icons/hicolor/64x64/apps
	icon64.files += ../data/64x64/$${TARGET}.png
}
	
