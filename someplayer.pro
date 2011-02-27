QMAKEVERSION = $$[QMAKE_VERSION]
ISQT4 = $$find(QMAKEVERSION, ^[2-9])
isEmpty( ISQT4 ) {
	error("Use the qmake include with Qt4.4 or greater, on Debian that is qmake-qt4");
}

TEMPLATE = subdirs
SUBDIRS  = src

desktop.path = /usr/share/applications/hildon
desktop.files = src/resources/someplayer.desktop

icon.path = /usr/share/icons/hicolor/scalable/hildon
icon.files = src/resources/someplayer.png

equalizer.path = /etc/.skel
equalizer.files = src/resources/equalizer.ini

translations.path = /opt/someplayer
translations.files = src/resources/*.qm

INSTALLS += icon desktop equalizer translations
