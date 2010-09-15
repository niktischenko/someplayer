#include "trackmetainformation.h"

using namespace SomePlayer::DataObjects;

TrackMetadata::TrackMetadata() {
}

TrackMetadata::TrackMetadata(QString title = "", QString artist = "", QString album = "", int length = 0) {
	_metadata["TITLE"] = title == "" ? "Unknown title" : title.trimmed();
	_metadata["ARTIST"] = artist == "" ? "Unknown artist" : artist.trimmed();
	_metadata["ALBUM"] = album == "" ? "Unknown album" : album.trimmed();
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
		return "Unknown title";
	}
}

QString TrackMetadata::artist() {
	if (_metadata.contains("ARTIST")) {
		return _metadata["ARTIST"];
	} else {
		return "Unknown artist";
	}
}

QString TrackMetadata::album() {
	if (_metadata.contains("ALBUM")) {
		return _metadata["ALBUM"];
	} else {
		return "Unknown album";
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
