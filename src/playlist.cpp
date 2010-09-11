#include "playlist.h"

using namespace SomePlayer::DataObjects;

Playlist::Playlist()
{
	_name = "New playlist";
}

Playlist::Playlist(const Playlist &playlist) {
	_name = playlist.name();
	_tracks = playlist.tracks();
}

Playlist& Playlist::operator =(const Playlist &playlist) {
	_name = playlist.name();
	_tracks = playlist.tracks();
	return *this;
}

QString Playlist::name() const {
	return _name;
}

const QList<Track> &Playlist::tracks() const {
	return _tracks;
}

void Playlist::setName(QString name) {
	_name = name;
}

void Playlist::addTrack(Track track) {
	_tracks.append(track);
}

