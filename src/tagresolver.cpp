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

#include "tagresolver.h"
#include <QFile>
#include <QFileInfo>
#include <tag.h>
#include <fileref.h>

using namespace SomePlayer::DataObjects;

TagResolver::TagResolver(QObject *parent) :
    QObject(parent)
{
}

void TagResolver::decode(QStringList files) {
	foreach (QString filename, files) {
		TagLib::FileRef file_ref(QFile::encodeName(filename).data());
		if (!file_ref.isNull()) {
			TagLib::Tag *tag = file_ref.tag();
			if (NULL != tag) {
				TagLib::AudioProperties *properties = file_ref.audioProperties();
				if (NULL != properties) {
					TrackMetadata meta(QString::fromStdWString(tag->title().toWString()),
							   QString::fromStdWString(tag->artist().toWString()),
							   QString::fromStdWString(tag->album().toWString()),
							   properties->length());
					Track track(0, meta, filename);
					emit decoded(track);
				}
			}
		} else { // workaround
			TrackMetadata meta;
			meta.setLength(0);
			QFileInfo fi(filename);
			meta.setArtist(fi.suffix().toUpper());
			meta.setTitle(fi.baseName());
			Track track(0, meta, filename);
			emit decoded(track);
		}
	}
	emit done();
}

void TagResolver::updateTags(Track track) {
	TagLib::FileRef file_ref(QFile::encodeName(track.source()).data());
	if (!file_ref.isNull()) {
		TagLib::Tag *tag = file_ref.tag();
		if (NULL != tag) {
			tag->setArtist(TagLib::String(track.metadata().artist().toStdWString()));
			tag->setAlbum(TagLib::String(track.metadata().album().toStdWString()));
			tag->setTitle(TagLib::String(track.metadata().title().toStdWString()));
		}
		file_ref.save();
	}
}
