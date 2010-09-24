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

#include "mediascanner.h"

using namespace SomePlayer::Storage;

#include <QMap>
#include <QDir>

MediaScanner::MediaScanner(QObject *parent) :
		QThread(parent), _stopped(false), _initialized(false)
{
	REGISTERED_FILE_EXTENSIONS << "mp3" << "flac" << "wma" << "acc" << "ogg";
}

void MediaScanner::run() {
	if (!_initialized)
		return;
	_foundMedia.clear();
	_scan_directory(_dir);
	emit scanFinish(_foundMedia);
	_stopped = true;
}

void MediaScanner::_scan_directory(QDir dir) {
	QFileInfoList items = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
	foreach (QFileInfo info, items) {
		if (info.isDir()) {
			QDir ndir(info.absoluteFilePath());
			_scan_directory(ndir);
		} else {
			QString suffix = info.suffix().toLower();
			if (REGISTERED_FILE_EXTENSIONS.contains(suffix)) {
				if (!_foundMedia.contains(info.absoluteFilePath()))
					_foundMedia << info.absoluteFilePath();
			}
		}
	}
}

void MediaScanner::stop() {
	_stopped = true;
	_initialized = false;
}

void MediaScanner::init(QString dir) {
	_stopped = false;
	_initialized = true;
	_dir = dir;
}
