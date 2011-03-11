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

#ifndef TRACK_H
#define TRACK_H

#include "someplayer.h"
#include "trackmetainformation.h"
#include "tagresolver.h"

// represents some track: metainformation + source url

namespace SomePlayer {
	namespace DataObjects {

		class Track : public QObject
		{
			Q_OBJECT

		public:
			Track();
			Track(const Track &track);
			Track(TrackMetadata metadata, QString source);
			Track(QString source);
			~Track();
			Track &operator=(const Track &track);
			TrackMetadata metadata() const; //read-write
			void setMetadata(TrackMetadata meta);
			QString source() const;
			void setSource (QString source);
			int count() const; //count of plays
			void setCount(int count); //for restoring from database and counting from player
			bool operator == (const Track &track) const;
			bool operator >= (const Track &track) const; // comparing only title
			bool operator < (const Track &track) const; // comparing only title
			QString mediaArtLocal();

		private:
			TrackMetadata _metadata;
			QString _source;
			int _count;
			TagResolver *_resolver;
		private slots:
			void decoded(Track);
		};

	};
};


#endif // TRACK_H
