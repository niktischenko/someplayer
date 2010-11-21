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

#include "library.h"

using namespace SomePlayer::DataObjects;
using namespace SomePlayer::Storage;

#include "mediascanner.h"
#include <QDir>
#include <QDebug>

Library::Library(QString databasePath, QString playlistsPath) : QObject(0) {
	_library_storage = new DbStorage(databasePath);
	_playlist_storage = new FileStorage(playlistsPath);
	_scanner = new MediaScanner();
	_resolver = new TagResolver(this);
	connect(_scanner, SIGNAL(scanFinish(QStringList)), this, SLOT(_scanned(QStringList)));
	connect(_resolver, SIGNAL(started()), this, SIGNAL(started()));
	connect(_resolver, SIGNAL(done()), this, SIGNAL(done()));
	connect(_resolver, SIGNAL(decoded(Track)), this, SLOT(_decoded(Track)));
}

Library::~Library() {
	delete _library_storage;
	delete _playlist_storage;
}

void Library::addDirectory(QString path, bool async) {
	_scanner->init(path);
	if (async) _scanner->start(QThread::LowestPriority);
	else _scanner->run();
}

void Library::addFile(QString path) {
	QStringList files(path);
	_resolver->decode(files);
}

QList<QString> Library::getArtists() {
	return _library_storage->getArtists();
}

QMap<QString, int> Library::getAlbumsForArtist(QString artist) {
	return _library_storage->getAlbumsForArtist(artist);
}

QList<Track> Library::getTracksForAlbum(QString album, QString artist) {
	return _library_storage->getTracksForAlbum(album, artist);
}

QList<Track> Library::getAllTracksForArtist(QString artist) {
	QList<Track> all;
	QMap<QString, int> albumsmap = getAlbumsForArtist(artist);
	QList<QString> albums = albumsmap.keys();
	foreach (QString album, albums) {
		all.append(getTracksForAlbum(album, artist));
	}
	return all;
}

// dynamic:

Playlist Library::getFavorites() {
	return _library_storage->getFavorites();
}

Playlist Library::getMostPlayed() {
	return _library_storage->getMostPlayed();
}

Playlist Library::getNeverPlayed() {
	return _library_storage->getNeverPlayed();
}

Playlist Library::getRecentlyAdded() {
	return _library_storage->getRecentlyAdded();
}


void Library::removeTrack(Track track) {
	_library_storage->removeTrack(track);
}

void Library::addTrack(Track track) {
	_library_storage->addTrack(track);
}

void Library::addToFavorites(Track track) {
	_library_storage->addToFavorites(track);
}

void Library::updateTrackCount(Track track) {
	_library_storage->updateTrackCount(track);
}


// playlists:

QList<Playlist> Library::getPlaylists() {
	return _playlist_storage->getPlaylists();
}

QStringList Library::getPlaylistsNames() {
	return _playlist_storage->getPlaylistsNames();
}

Playlist Library::getPlaylist(QString name) {
	return _playlist_storage->getPlaylist(name);
}

void Library::savePlaylist(const Playlist &playlist) {
	_playlist_storage->savePlaylist(playlist);
}

void Library::removePlaylist(const Playlist &playlist) {
	_playlist_storage->removePlaylist(playlist);
}

void Library::removePlaylist(QString name) {
	_playlist_storage->removePlaylist(name);
}

Playlist Library::getCurrentPlaylist() {
	return _playlist_storage->getCurrentPlaylist();
}

void Library::saveCurrentPlaylist(const Playlist &playlist) {
	_playlist_storage->saveCurrentPlaylist(playlist);
}

void Library::_decoded(Track track) {
	emit tick();
	addTrack(track);
}

void Library::_scanned(QStringList files) {
	emit allCount(files.count());
	_resolver->decode(files);
}

void Library::updateTrackMetadata(Track track) {
	Track ntrack = _library_storage->updateTrack(track);
	// update all playlists
	QList<QString> playlists = getPlaylistsNames();
	foreach (QString name, playlists) {
		Playlist pl = getPlaylist(name);
		QList<Track> tracks = pl.tracks();
		int pos = tracks.indexOf(ntrack); // comparing using source
		tracks.removeOne(ntrack); // comparing using source
		tracks.insert(pos, ntrack);
		pl.setTracks(tracks);
		savePlaylist(pl);
	}
	_resolver->updateTags(ntrack);
}

QList<Track> Library::searchTracks(QString pattern) {
	return _library_storage->searchTracks(pattern);
}

QList<QString> Library::getDirectories() {
	return _library_storage->getDirectories();
}

int Library::getArtistsCount() {
	return _library_storage->getArtistsCount();
}

int Library::getAlbumsCount() {
	return _library_storage->getAlbumsCount();
}

int Library::getTracksCount() {
	return _library_storage->getTracksCount();
}

void Library::updateAll() {
	QList<QString> directories = _library_storage->getDirectories();
	updateDirectories(directories);
}

void Library::updateDirectories(QList<QString> directories) {
	foreach (QString directory, directories) {
		_library_storage->checkTracksFrom(directory);
		addDirectory(directory, false);
	}
}

void Library::deleteDirectories(QList<QString> directories) {
	foreach (QString directory, directories) {
		_library_storage->deleteTracksFrom(directory);
	}
}

void Library::updatePlaylists() {
	emit started();
	QList<QString> playlists = getPlaylistsNames();
	emit allCount(playlists.count());
	foreach (QString name, playlists) {
		Playlist playlist = getPlaylist(name);
		QList<Track> tracks = playlist.tracks();
		QList<Track> ntracks;
		foreach (const Track &track, tracks) {
			if (QFile::exists(track.source())) {
				ntracks.append(track);
			}
		}
		playlist.setTracks(ntracks);
		savePlaylist(playlist);
		emit tick();
	}
	emit done();
}

void Library::removeFromFavorites(Track track) {
	_library_storage->removeFromFavorites(track);
}

bool Library::isFavorite(Track track) {
	return _library_storage->isFavorite(track);
}
