#ifndef LIBRARY
#define LIBRARY

#include "someplayer.h"
#include "track.h"
#include "playlist.h"
#include "dbstorage.h"
#include "filestorage.h"

// represents media library: tracks, playlists
// it uses different media storages for tracks and playlists
// but dynamic playlits will be stored with tracks into the same storage

using SomePlayer::DataObjects::Track;
using SomePlayer::DataObjects::Playlist;
using SomePlayer::Storage::DbStorage;
using SomePlayer::Storage::FileStorage;

namespace SomePlayer {
	namespace DataObjects {

		class Library {
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

			void removeTrack(Track);
			void addTrack(Track);
			void addToFavorites(Track);

			void updateTrack(Track);

			QList<Playlist> getPlaylists();
			void savePlaylist(Playlist playlist);
			void removePlaylist(Playlist playlist);

			Playlist getCurrentPlaylist();
			void saveCurrentPlaylist(Playlist playlist);

		private:
			DbStorage *_library_storage;
			FileStorage *_playlist_storage;
		};

	};
};

#endif
