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

#ifndef TAGRESOLVER_H
#define TAGRESOLVER_H

#include <QObject>
#include "someplayer.h"
#include "track.h"
#include <phonon/MediaObject>
#include <phonon/MediaSource>

using SomePlayer::DataObjects::Track;

namespace SomePlayer {
	namespace DataObjects {

		class TagResolver : public QObject
		{

			Q_OBJECT
		public:
			explicit TagResolver(QObject *parent = 0);
		public slots:
			void decode (QStringList files);

		signals:
			void decoded(Track);
			void done();

		private slots:
			void metaStateChanged(Phonon::State newState, Phonon::State /*oldState*/);
		private:
			QStringList _files;
			Phonon::MediaObject *_metaObject;
			QList<Phonon::MediaSource> _sources;

		};
	};
};
#endif // TAGRESOLVER_H
