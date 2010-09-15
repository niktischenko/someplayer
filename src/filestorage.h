#ifndef FILE_STORAGE
#define FILE_STORAGE

#include "someplayer.h"
#include "storage.h"
#include "playlist.h"
#include <QRegExp>

#define _CURRENT_PLAYLIST_NAME_ "___current"
#define _PLAYLIST_FILE_EXTENSION_ "spls"
#define _PLAYLIST_SIGNATURE_ "#SOMEPLAYLIST"
#define _PLAYLIST_META_KEYWORD_ "#META"
#define _PLAYLIST_PATH_KEYWORD_ "#PATH"

// format:
/*
 #SOMEPLAYLIST
 #META [ID][seconds],::artist::,::album::,::title::
 #PATH file_path
 #META [ID][seconds],::artist::,::album::,::title::
 #PATH file_path
 ...
 */

// represents file-level storage
// it store data into separate files (e.g. playlist)

using SomePlayer::DataObjects::Playlist;

namespace SomePlayer {
	namespace Storage {

		class FileStorage {
		public:
			FileStorage(QString path);

			QList<Playlist> getPlaylists();
			QStringList getPlaylistsNames();
			Playlist getPlaylist(QString name);
			void savePlaylist(Playlist playlist);
			void removePlaylist(Playlist playlist);
			void removePlaylist(QString name);

			Playlist getCurrentPlaylist();
			void saveCurrentPlaylist(Playlist playlist);
		private:
			QString _path_prefix;
			QRegExp _meta_regexp;
			QRegExp _path_regexp;
		};

	};
};

#endif
