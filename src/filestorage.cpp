#include "filestorage.h"
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QTextStream>
#include <QRegExp>
#include <QDebug>

using namespace SomePlayer::Storage;
using namespace SomePlayer::DataObjects;

FileStorage::FileStorage(QString path) {
	_path_prefix = path;
	_meta_regexp.setPattern("#META\\ +\\[(\\d+)\\]\\[(\\d+)\\].*::(.+)::,::(.+)::,::(.+)::");
	_path_regexp.setPattern("#PATH (.+)");

	Playlist current = getCurrentPlaylist();
	if (current.name() == PLAYLIST_BAD_NAME) {
		current.setName(_CURRENT_PLAYLIST_NAME_);
		saveCurrentPlaylist(current);
	}
}

QList<Playlist> FileStorage::getPlaylists() {
	QList<Playlist> stub;
	return stub;
}

Playlist FileStorage::getPlaylist(QString name) {
	if (name == _CURRENT_PLAYLIST_SUBST_)
		name = _CURRENT_PLAYLIST_NAME_;
	QFile playlistFile (_path_prefix+"/"+name+"."+_PLAYLIST_FILE_EXTENSION_);
	Playlist playlist;
	playlist.setName(PLAYLIST_BAD_NAME);
	if (playlistFile.exists()) {
		playlist.setName(name);
		playlistFile.open(QFile::ReadOnly);
		QTextStream stream(&playlistFile);
		QString buffer = stream.readLine();
		if (buffer.startsWith(_PLAYLIST_SIGNATURE_)) {
			while (!stream.atEnd()) {
				buffer = stream.readLine();
				if (_meta_regexp.indexIn(buffer) != -1) {
					int id = _meta_regexp.cap(1).toInt();
					int seconds = _meta_regexp.cap(2).toInt();
					QString artist = _meta_regexp.cap(3);
					QString album = _meta_regexp.cap(4);
					QString title = _meta_regexp.cap(5);
					buffer = stream.readLine();
					if (_path_regexp.indexIn(buffer) != -1) {
						QString source = _path_regexp.cap(1);
						TrackMetadata meta(title, artist, album, seconds);
						Track track(id, meta, source);
						playlist.addTrack(track);
					}
				}
			}
		}
	}
	return playlist;
}

QStringList FileStorage::getPlaylistsNames() {
	QDir directory(_path_prefix);
	QDirIterator iterator(directory, QDirIterator::FollowSymlinks);
	QStringList playlistNames;
	while (iterator.hasNext()) {
		QString entry = iterator.next();
		QFileInfo info(entry);
		QString suffix = info.suffix().toLower();
		if (suffix == _PLAYLIST_FILE_EXTENSION_) {
			QString name = info.fileName().replace(QString(".%1").arg(_PLAYLIST_FILE_EXTENSION_), "", Qt::CaseInsensitive);
			if (name == _CURRENT_PLAYLIST_NAME_)
			    name = _CURRENT_PLAYLIST_SUBST_;
			playlistNames.append(name);
		}
	}
	return playlistNames;
}

void FileStorage::savePlaylist(Playlist playlist) {
	QString name = playlist.name();
	if (playlist.name() == _CURRENT_PLAYLIST_SUBST_)
		name = _CURRENT_PLAYLIST_NAME_;
	QString filename = _path_prefix + "/" +name+"."_PLAYLIST_FILE_EXTENSION_;
	QFile playlistFile(filename);
	if (playlistFile.exists()) {
		playlistFile.remove();
	}
	playlistFile.open(QFile::WriteOnly);
	QTextStream stream(&playlistFile);
	stream << _PLAYLIST_SIGNATURE_ << endl;
	const QList<Track> &tracks = playlist.tracks();
	foreach (Track track, tracks) {
		stream << _PLAYLIST_META_KEYWORD_ << " [" << track.id() << "]" << "[" << track.metadata().length() << "],::"
				<< track.metadata().artist() << "::,::" << track.metadata().album() << "::,::"
				<< track.metadata().title() << "::" << endl;
		stream << _PLAYLIST_PATH_KEYWORD_ << " " << track.source() << endl;
	}
}

void FileStorage::removePlaylist(Playlist playlist) {
	QString filename = _path_prefix + "/" + playlist.name() + "." + _PLAYLIST_FILE_EXTENSION_;
	QFile file(filename);
	file.remove();
}

void FileStorage::removePlaylist(QString name) {
	QString filename = _path_prefix + "/" + (name == _CURRENT_PLAYLIST_SUBST_ ? _CURRENT_PLAYLIST_NAME_ : name)
			   + "." + _PLAYLIST_FILE_EXTENSION_;
	QFile file(filename);
	file.remove();
}

Playlist FileStorage::getCurrentPlaylist() {
	return getPlaylist(_CURRENT_PLAYLIST_NAME_);
}

void FileStorage::saveCurrentPlaylist(Playlist playlist) {
	playlist.setName(_CURRENT_PLAYLIST_NAME_);
	savePlaylist(playlist);
}
