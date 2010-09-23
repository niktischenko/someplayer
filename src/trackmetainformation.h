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

#ifndef TRACK_METAINFORMATION
#define TRACK_METAINFORMATION

#include "someplayer.h"

// represents track metainformation only.
// now it contains TITLE, ARTIST, ALBUM, GENRE

#define _UNKNOWN_ARTIST_ "Unknown artist"
#define _UNKNOWN_ALBUM_  "Unknown album"
#define _UNKNOWN_TRACK_  "Unknown track"

namespace SomePlayer {
	namespace DataObjects {
		class TrackMetadata {

		public:
			TrackMetadata();
			TrackMetadata(QString title, QString artist, QString album, int length);
			TrackMetadata(const TrackMetadata &metadata);

			QString title();
			QString artist();
			QString album();
			int length();

			void setTitle(QString title);
			void setArtist(QString artist);
			void setAlbum(QString album);
			void setLength(int length);

		private:
			QMap<QString, QString> _metadata;
			int _length;
		};
	};
};

#endif
