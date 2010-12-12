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

#ifndef PLAYLIST
#define PLAYLIST

#include "track.h"
#include "someplayer.h"

#define PLAYLIST_BAD_NAME "Bad playlist"
#define PLAYLIST_CURRENT_NAME "Current"

// represents playlist: list of tracks, name

namespace SomePlayer {
	namespace DataObjects {

		struct LastPlayed {
			int trackId;
			int position;
		};

		class Playlist {
		public:
			Playlist();
			Playlist(const Playlist &playlist);

			QString name();
			QList<Track> tracks();

			void setName(QString name);
			void addTrack(Track track);
			void removeTrack(Track track);
			void removeTrackAt(int id);
			void setTracks(QList<Track>);
			void clear();

		private:
			QString _name;
			QList<Track> _tracks;
		};
	};
};

#endif
