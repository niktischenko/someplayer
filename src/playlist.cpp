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

void Playlist::removeTrackAt(int id) {
	_tracks.removeAt(id);
}

void Playlist::clear() {
	_tracks.clear();
}
