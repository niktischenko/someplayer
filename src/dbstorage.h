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
			QList<QString> getArtists();
			QList<QString> getAlbumsForArtist(QString artist);
			QList<Track> getTracksForAlbum(QString album, QString artist); // hm...

			Playlist getFavorites();
			Playlist getMostPlayed();
			Playlist getNeverPlayed();
			Playlist getRecentlyAdded();

			void removeTrack(Track track);
			void addToFavorites(Track track);

			void updateTrack(Track track);
			void addTrack(Track track);

		private:
			QSqlDatabase db;
			void _create_database_structure();
			void _prepare_queries();

			int _check_add_artist(QString artist);
			int _check_add_album(QString album, int artist_id);

			// queries
			QSqlQuery *_get_artists_query;
			QSqlQuery *_get_albums_for_artist_query;
			QSqlQuery *_get_tracks_for_album_query;
			QSqlQuery *_get_favorites_query;
			QSqlQuery *_get_most_played_query;
			QSqlQuery *_get_never_played_query;
			QSqlQuery *_get_recently_added_query;

			QSqlQuery *_check_artist_query;
			QSqlQuery *_check_album_query;
			QSqlQuery *_check_track_query;

			QSqlQuery *_insert_artist_query;
			QSqlQuery *_insert_album_query;

			QSqlQuery *_insert_track_query;
			QSqlQuery *_insert_date_query;
		};
	};
};

#endif
