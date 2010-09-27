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

#ifndef LIBRARY
#define LIBRARY

#include "someplayer.h"
#include "track.h"
#include "playlist.h"
#include "dbstorage.h"
#include "filestorage.h"
#include "mediascanner.h"
#include "tagresolver.h"

// represents media library: tracks, playlists
// it uses different media storages for tracks and playlists
// but dynamic playlits will be stored with tracks into the same storage

using SomePlayer::DataObjects::Track;
using SomePlayer::DataObjects::Playlist;
using SomePlayer::Storage::DbStorage;
using SomePlayer::Storage::FileStorage;
using SomePlayer::Storage::MediaScanner;

namespace SomePlayer {
	namespace DataObjects {

		class Library : public QObject {
			Q_OBJECT
		public:
			Library(QString databasePath, QString playlistsPath);
			~Library();

			void addDirectory(QString path);
			void addFile(QString path);

			QList<QString> getArtists();
			QList<QString> getAlbumsForArtist(QString artist);
			QList<Track> getTracksForAlbum(QString album, QString artist);

			Playlist getFavorites();
			Playlist getMostPlayed();
			Playlist getNeverPlayed();
			Playlist getRecentlyAdded();

			QList<Playlist> getPlaylists();
			QStringList getPlaylistsNames();
			Playlist getPlaylist(QString name);
			void savePlaylist(const Playlist &playlist);
			void removePlaylist(const Playlist &playlist);
			void removePlaylist(QString name);

			Playlist getCurrentPlaylist();
			void saveCurrentPlaylist(const Playlist &playlist);

		signals:
			void done();
			void busy(QString);
			void addingTracks(int);
			void trackAdded();

		private:
			DbStorage *_library_storage;
			FileStorage *_playlist_storage;
			MediaScanner *_scanner;
			TagResolver *_resolver;

		private slots:
			void _scanned(QStringList);
			void _decoded(Track);

		public slots:
			void removeTrack(Track);
			void addTrack(Track);
			void addToFavorites(Track);
			void updateTrackCount(Track);
			void updateTrackMetadata(Track);
		};

	};
};

#endif
