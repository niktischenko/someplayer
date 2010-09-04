#include "filestorage.h"

using namespace SomePlayer::Storage;

FileStorage::FileStorage(QString path) {
	_path_prefix = path;
}

QList<Playlist> FileStorage::getPlaylists() {
	QList<Playlist> stub;
	return stub;
}

void FileStorage::savePlaylist(Playlist playlist) {
}

void FileStorage::removePlaylist(Playlist playlist) {
}

void FileStorage::removePlaylist(QString name) {
}

Playlist FileStorage::getCurrentPlaylist() {
	Playlist stub;
	return stub;
}

void FileStorage::saveCurrentPlaylist(Playlist playlist) {
}
