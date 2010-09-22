/*
 * SomePlayer - An alternate music player for Maemo 5
 * Copyright (C) 2010 Nikolay (somebody) Tischenko <niktischenko@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

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
