#include "trackmetainformation.h"

using namespace SomePlayer::DataObjects;

TrackMetadata::TrackMetadata() {
}

TrackMetadata::TrackMetadata(QString title = "", QString artist = "", QString album = "", QString genre = "") {
	_metadata["TITLE"] = title;
	_metadata["ARTIST"] = artist;
	_metadata["ALBUM"] = album;
	_metadata["GENRE"] = genre;
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

QString TrackMetadata::genre() {
	if (_metadata.contains("GENRE")) {
		return _metadata["GENRE"];
	} else {
		return "Unknown genre";
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

void TrackMetadata::setGenre(QString genre) {
	_metadata["GENRE"] = genre;
}
