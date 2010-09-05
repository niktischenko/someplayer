#include "library.h"

using namespace SomePlayer::DataObjects;
using namespace SomePlayer::Storage;

#include "mediascanner.h"
#include <QDir>

Library::Library(QString databasePath, QString playlistsPath) : QObject(0) {
	_library_storage = new DbStorage(databasePath);
	_playlist_storage = new FileStorage(playlistsPath);
	_scanner = new MediaScanner();
	_resolver = new TagResolver(this);
	connect(_scanner, SIGNAL(scanFinish(QStringList)), _resolver, SLOT(decode(QStringList)));
	connect(_resolver, SIGNAL(decoded(Track)), this, SLOT(addTrack(Track)));
}

Library::~Library() {
	delete _library_storage;
	delete _playlist_storage;
}

void Library::addDirectory(QString path) {
	_scanner->init(path);
	_scanner->start(QThread::LowestPriority);
}

void Library::addFile(QString path) {
	QStringList files(path);
	_resolver->decode(files);
}

QList<QString> Library::getArtists() {
	return _library_storage->getArtists();
}

QList<QString> Library::getAlbumsForArtist(QString artist) {
	return _library_storage->getAlbumsForArtist(artist);
}

QList<Track> Library::getTracksForAlbum(QString album, QString artist) {
	return _library_storage->getTracksForAlbum(album, artist);
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

void Library::savePlaylist(Playlist playlist) {
	_playlist_storage->savePlaylist(playlist);
}

void Library::removePlaylist(Playlist playlist) {
	_playlist_storage->removePlaylist(playlist);
}

void Library::removePlaylist(QString name) {
	_playlist_storage->removePlaylist(name);
}

Playlist Library::getCurrentPlaylist() {
	return _playlist_storage->getCurrentPlaylist();
}

void Library::saveCurrentPlaylist(Playlist playlist) {
	_playlist_storage->saveCurrentPlaylist(playlist);
}
