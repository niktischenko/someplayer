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

#ifndef DB_STORAGE
#define DB_STORAGE

#include "someplayer.h"
#include "storage.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include "track.h"
#include "playlist.h"

#define _DATABASE_NAME_ "/library.sqlite3"

// represents database storage
// it store date into some database (e.g. tracks)

using SomePlayer::DataObjects::Playlist;
using SomePlayer::DataObjects::Track;

namespace SomePlayer {
	namespace Storage {

		class DbStorage {
		public:
			DbStorage(QString path);
			~DbStorage();
			QList<QString> getDirectories();
			QList<QString> getArtists();
			QMap<QString, int> getAlbumsForArtist(QString artist);
			QList<Track> getTracksForAlbum(QString album, QString artist); // hm...

			Playlist getFavorites();
			Playlist getMostPlayed();
			Playlist getNeverPlayed();
			Playlist getRecentlyAdded();

			QList<Track> search(QString pattern);

			void removeTrack(Track track);
			void addToFavorites(Track track);

			void updateTrackCount(Track track);
			Track updateTrack(Track);
			void addTrack(Track track);

		private:
			QSqlDatabase db;
			void _create_database_structure();
			void _prepare_queries();

			int _check_add_artist(QString artist);
			int _check_add_album(QString album, int artist_id, int year);
			int _check_add_directory(QString path);

			void _cleanup();

			// queries
			QSqlQuery *_get_artists_query;
			QSqlQuery *_get_albums_for_artist_sort_name_query;
			QSqlQuery *_get_albums_for_artist_sort_year_query;
			QSqlQuery *_get_tracks_for_album_query;
			QSqlQuery *_get_favorites_query;
			QSqlQuery *_get_most_played_query;
			QSqlQuery *_get_never_played_query;
			QSqlQuery *_get_recently_added_query;
			QSqlQuery *_get_track_count;
			QSqlQuery *_get_tracks_by_pattern_query;
			QSqlQuery *_get_track_id_by_source_query;
			QSqlQuery *_get_directories_query;

			QSqlQuery *_check_artist_query;
			QSqlQuery *_check_album_query;
			QSqlQuery *_check_directory_query;

			QSqlQuery *_insert_artist_query;
			QSqlQuery *_insert_album_query;
			QSqlQuery *_insert_track_query;
			QSqlQuery *_insert_date_query;
			QSqlQuery *_insert_favorites_query;
			QSqlQuery *_insert_directory_query;

			QSqlQuery *_update_track_count_query;

			QSqlQuery *_remove_track_query;
			QSqlQuery *_remove_empty_artists_query;
			QSqlQuery *_remove_empty_albums_query;
		};
	};
};

#endif
