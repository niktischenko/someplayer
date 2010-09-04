#include "trackmetainformation.h"

using namespace SomePlayer::DataObjects;

TrackMetadata::TrackMetadata() {
}

TrackMetadata::TrackMetadata(QString title = "", QString artist = "", QString album = "") {
	_metadata["TITLE"] = title;
	_metadata["ARTIST"] = artist;
	_metadata["ALBUM"] = album;
}

TrackMetadata::TrackMetadata(const TrackMetadata &metadata) {
	this->_metadata = metadata._metadata;
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

void TrackMetadata::setTitle(QString title) {
	_metadata["TITLE"] = title;
}

void TrackMetadata::setArtist(QString artist) {
	_metadata["ARTIST"] = artist;
}

void TrackMetadata::setAlbum(QString album) {
	_metadata["ALBUM"] = album;
}
