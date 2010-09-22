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

#ifndef FILE_STORAGE
#define FILE_STORAGE

#include "someplayer.h"
#include "storage.h"
#include "playlist.h"
#include <QRegExp>

#define _CURRENT_PLAYLIST_NAME_ "___current"
#define _CURRENT_PLAYLIST_SUBST_ "Now playing"
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
