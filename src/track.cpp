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

#include "track.h"
#include "tagresolver.h"
#include <QFileInfo>
#include <QCryptographicHash>
#include <QDebug>

using namespace SomePlayer::DataObjects;

Track::Track() : QObject() {
}

Track::Track(TrackMetadata metadata, QString source) : QObject() {
	_metadata = metadata;
	if (_metadata.title() == _UNKNOWN_TRACK_) {
		QFileInfo info(source);
		_metadata.setTitle(info.baseName());
	}
	_source = source;
	_count = 0;
}

Track::Track(const Track &track) : QObject() {
	this->_metadata = track.metadata();
	this->_source = track.source();
	this->_count = track._count;
}

/// deprecated
Track::Track(QString source) :QObject() {
	_resolver = new TagResolver(this);
	connect(_resolver, SIGNAL(decoded(Track)), this, SLOT(decoded(Track)));
	QStringList foo;
	foo << source;
	_resolver->decode(foo);
	_count = 0;
}

TrackMetadata Track::metadata() const {
	return _metadata;
}

void Track::setMetadata(TrackMetadata meta) {
	_metadata = meta;
}

QString Track::source() const {
	return _source;
}

void Track::setSource(QString source) {
	_source = source;
}

int Track::count() const{
	return _count;
}

void Track::setCount(int count) {
	_count = count;
}

void Track::decoded(Track track) {
	_source = track.source();
	_metadata = track.metadata();
	delete _resolver;
}

Track &Track::operator =(const Track &track) {
	_source = track.source();
	_metadata = track.metadata();
	_count = track._count;
	return *this;
}

Track::~Track() {}

bool Track::operator ==(const Track &track) const {
	return _source == track._source;
}

bool Track::operator >= (const Track &track) const {
	return (metadata().title().compare(track.metadata().title()) >= 0);
}

bool Track::operator < (const Track &track) const {
	return (metadata().title().compare(track.metadata().title()) < 0);
}

QString Track::mediaArtLocal() {
	QString album = _metadata.album();
	if (album == _UNKNOWN_ALBUM_) {
		return QString("no_mediaartlocal");
	}
	QRegExp rsb("\\[.*\\]");
	QRegExp rfb("{.*}");
	QRegExp rrb("\\(.*\\)");
	QRegExp stripB("^[()_{}[]!@#$^&*+=|\\\\/\"'?<>~`\\s\\t]*");
	QRegExp stripE("[()_{}[]!@#$^&*+=|\\\\/\"'?<>~`\\s\\t]*$");
	album = album.replace(rsb, "");
	album = album.replace(rfb, "");
	album = album.replace(rrb, "");
	album = album.replace(stripB, "");
	album = album.replace(stripE, "");
	album = album.replace("  ", " ");
	album = album.replace("\t", " ");
	album = album.toLower();
	QByteArray first_hash = QCryptographicHash::hash(QString(" ").toUtf8(), QCryptographicHash::Md5).toHex();
	QByteArray second_hash = QCryptographicHash::hash(QString(album).toUtf8(), QCryptographicHash::Md5).toHex();
	return QString("album-%1-%2.jpeg").arg(first_hash.constData()).arg(second_hash.constData());
}
