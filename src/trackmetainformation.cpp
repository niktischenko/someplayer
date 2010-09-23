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

#include "trackmetainformation.h"

using namespace SomePlayer::DataObjects;

TrackMetadata::TrackMetadata() {
}

TrackMetadata::TrackMetadata(QString title = "", QString artist = "", QString album = "", int length = 0) {
	_metadata["TITLE"] = title == "" ? _UNKNOWN_TRACK_ : title.trimmed();
	_metadata["ARTIST"] = artist == "" ? _UNKNOWN_ARTIST_ : artist.trimmed();
	_metadata["ALBUM"] = album == "" ? _UNKNOWN_ALBUM_ : album.trimmed();
	_length = length;
}

TrackMetadata::TrackMetadata(const TrackMetadata &metadata) {
	this->_metadata = metadata._metadata;
	this->_length = metadata._length;
}

QString TrackMetadata::title() {
	if (_metadata.contains("TITLE")) {
		return _metadata["TITLE"];
	} else {
		return _UNKNOWN_TRACK_;
	}
}

QString TrackMetadata::artist() {
	if (_metadata.contains("ARTIST")) {
		return _metadata["ARTIST"];
	} else {
		return _UNKNOWN_ARTIST_;
	}
}

QString TrackMetadata::album() {
	if (_metadata.contains("ALBUM")) {
		return _metadata["ALBUM"];
	} else {
		return _UNKNOWN_ALBUM_;
	}
}

int TrackMetadata::length() {
	return _length;
}

void TrackMetadata::setTitle(QString title) {
	_metadata["TITLE"] = title;
}

void TrackMetadata::setArtist(QString artist) {
	_metadata["ARTIST"] = artist;
}

void TrackMetadata::setAlbum(QString album) {
	_metadata["ALBUM"] = album;
}

void TrackMetadata::setLength(int length) {
	_length = length;
}
