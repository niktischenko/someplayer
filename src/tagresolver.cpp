#include "tagresolver.h"
#include <QDebug>

using namespace SomePlayer::DataObjects;

TagResolver::TagResolver(QObject *parent) :
    QObject(parent)
{
	_metaObject = new Phonon::MediaObject(this);
	connect(_metaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(metaStateChanged(Phonon::State,Phonon::State)));
}

void TagResolver::decode(QStringList files) {
	_sources.clear();
	foreach (QString filename, files) {
		_sources.append(Phonon::MediaSource(filename));
	}
	if (!_sources.isEmpty()) {
		_metaObject->setCurrentSource(_sources.at(0));
	}
}

void TagResolver::metaStateChanged(Phonon::State newState, Phonon::State /*oldState*/) {
	if (newState == Phonon::StoppedState) {
		int time = _metaObject->totalTime();
		Phonon::MediaSource source = _metaObject->currentSource();
		QMap<QString, QString> meta = _metaObject->metaData();
		TrackMetadata metadata(meta.value("TITLE"), meta.value("ARTIST"), meta.value("ALBUM"), time/1000);
		Track track(0, metadata, source.fileName());
		int index = _sources.indexOf(source)+1;
		emit decoded(track);
		if (index != _sources.size()) {
			Phonon::MediaSource newSource = _sources.at(index);
			_metaObject->clear();
			_metaObject->setCurrentSource(newSource);
		} else {
			emit done();
		}
	} else if (newState == Phonon::ErrorState) {
		Phonon::MediaSource s = _metaObject->currentSource();
		_metaObject->clear();
		_metaObject->setCurrentSource(s);
	}
}
