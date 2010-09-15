#include "playlist.h"
#include <QDebug>

using namespace SomePlayer::DataObjects;

Playlist::Playlist()
{
	_name = "New playlist";
}

Playlist::Playlist(const Playlist &playlist) {
	_name = playlist._name;
	_tracks = playlist._tracks;
}

QString Playlist::name() {
	return _name;
}

QList<Track> Playlist::tracks() {
	return _tracks;
}

void Playlist::setName(QString name) {
	_name = name;
}

void Playlist::addTrack(Track track) {
	if (!_tracks.contains(track))
		_tracks.append(track);
}

void Playlist::removeTrack(Track track) {
	_tracks.removeOne(track);
}

