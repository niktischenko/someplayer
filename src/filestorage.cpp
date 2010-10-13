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

#include "filestorage.h"
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QTextStream>
#include <QtXml/QtXml>

using namespace SomePlayer::Storage;
using namespace SomePlayer::DataObjects;

FileStorage::FileStorage(QString path) {
	_path_prefix = path;

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
		QDomDocument doc;
		playlistFile.open(QFile::ReadOnly);
		doc.setContent(&playlistFile);
		playlistFile.close();
		QDomElement eplaylist = doc.documentElement();
		if (eplaylist.tagName() == "playlist") {
			QDomElement etracklist = eplaylist.firstChildElement("trackList");
			if (!etracklist.isNull()) {
				QDomElement etrack = etracklist.firstChildElement("track");
				while (!etrack.isNull()) {
					QDomElement elocation = etrack.firstChildElement("location");
					QDomElement eextension = etrack.firstChildElement("extension");
					if (!eextension.isNull()) {
						QDomElement ecl_clip = eextension.firstChildElement("cl:clip");
						if (!ecl_clip.isNull()) {
							QString artist = ecl_clip.attribute("artist");
							QString album = ecl_clip.attribute("album");
							QString title = ecl_clip.attribute("title");
							QDomElement eduration = etrack.firstChildElement("duration");
							if (!eduration.isNull()) {
								QVariant duration = eduration.text();
								QByteArray basource;
								basource.append(elocation.text());
								QString source = QUrl::fromEncoded(basource).toLocalFile();
								TrackMetadata meta(title, artist, album, duration.toInt()/1000);
								Track track(0, meta, source);
								playlist.addTrack(track);
							}
						}
					}
					etrack = etrack.nextSiblingElement("track");
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
	QDomDocument doc;
	QDomElement root = doc.createElement("playlist");
	root.setAttribute("version", "1");
	root.setAttribute("xmlns", "http://xspf.org/ns/0/");
	root.setAttribute("xmlns:cl", "http://example.com");
	QDomElement tracklist = doc.createElement("trackList");
	foreach (Track track, playlist.tracks()) {
		QDomElement etrack = doc.createElement("track");
		QDomElement elocation = doc.createElement("location");
		QDomElement eextension = doc.createElement("extension");
		QDomElement ecl_clip = doc.createElement("cl:clip");
		QDomElement etitle = doc.createElement("title");
		QDomElement eduration = doc.createElement("duration");
		etitle.appendChild(doc.createTextNode(QString("%1 - %2").arg(track.metadata().artist()).arg(track.metadata().title())));
		ecl_clip.setAttribute("title", track.metadata().title());
		ecl_clip.setAttribute("artist", track.metadata().artist());
		ecl_clip.setAttribute("album", track.metadata().album());
		eextension.appendChild(ecl_clip);
		eextension.setAttribute("application", "http://example.com");
		elocation.appendChild(doc.createTextNode(QString("%1").arg(QUrl::fromLocalFile(track.source()).toEncoded().constData())));
		eduration.appendChild(doc.createTextNode(QString("%1").arg(track.metadata().length()*1000)));
		etrack.appendChild(elocation);
		etrack.appendChild(eextension);
		etrack.appendChild(etitle);
		etrack.appendChild(eduration);
		tracklist.appendChild(etrack);
	}
	root.appendChild(tracklist);
	doc.appendChild(root);
	stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	stream << doc.toString();
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
