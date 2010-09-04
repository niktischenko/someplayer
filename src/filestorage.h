#ifndef FILE_STORAGE
#define FILE_STORAGE

#include "someplayer.h"
#include "storage.h"
#include "playlist.h"

#define _CURRENT_PLAYLIST_NAME_ "___current"
#define _PLAYLIST_FILE_EXTENSION_ ".m3u"

// represents file-level storage
// it store data into separate files (e.g. playlist)

using SomePlayer::DataObjects::Playlist;

namespace SomePlayer {
	namespace Storage {

		class FileStorage : public Storage {
		public:
			FileStorage(QString path);

			QList<Playlist> getPlaylists();
			void savePlaylist(Playlist playlist);
			void removePlaylist(Playlist playlist);
			void removePlaylist(QString name);

			Playlist getCurrentPlaylist();
			void saveCurrentPlaylist(Playlist playlist);
		private:
			QString _path_prefix;
		};

	};
};

#endif
