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

#ifndef SOMEPLAYER
#define SOMEPLAYER

// common settings, constants and so on

namespace SomePlayer {
	namespace DataObjects {
		class Track;
		class TrackMetadata;
		class TagResolver;
		class Playlist;
		class Library;
	};
	namespace Storage {
	};
	namespace Playback {
	};
};

// common includes

#include <QObject>
#include <QString>
#include <QStringList>
#include <QList>
#include <QMap>
#include <QUrl>
#include <QMessageBox>
#include "config.h"

#define _DYNAMIC_PLAYLIST_MAX_COUNT_ 50
#define _SOMEPLAYER_VERSION_ "1.5.1"
#define NDEBUG

#define _SERVICE_NAME_ "ru.somebody.someplayer"

#define _APPLICATION_PATH_ "/opt/someplayer"

#define CONFIRM_ACTION(PARENT, TEXT) \
	if (QMessageBox::question(PARENT, tr("Confirm action"), \
				  TEXT, QMessageBox::Ok, QMessageBox::Cancel) != QMessageBox::Ok) \
	{return;}

#endif
