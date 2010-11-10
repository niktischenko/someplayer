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


#include "coverfinder.h"
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QtConcurrentRun>
#include <mpegfile.h>
#include <flacfile.h>
#include <oggfile.h>
#include <id3v2tag.h>
#include <mpeg/id3v2/frames/attachedpictureframe.h>

CoverFinder::CoverFinder(QObject *parent) :
		QObject(parent)
{
	_defaultCover = QImage(":/images/defaultcover.png");
	SUFFIXES << "png" << "jpg" << "jpeg" << "bmp" << "gif";
	NAMES << "cover" << "folder" << "album";
	DIRS << "cover" << "folder" << ".cover" << ".folder";
}

bool CoverFinder::_find(QString path) {
	QDir dir(path);
	QFileInfoList items = dir.entryInfoList(QDir::Files);
	QFileInfoList dirs = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Hidden);
	QFileInfoList pics;
	foreach (QFileInfo item, items) {
		if (SUFFIXES.contains(item.suffix().toLower())) {
			pics << item;
			if (NAMES.contains(item.baseName().toLower())) {
				emit found(QImage(item.absoluteFilePath()));
				return true;
			}
		}
	}
	foreach (QFileInfo item, pics) {
		QImage i(item.absoluteFilePath());
		if (0.8 <= (i.width()*1.0/(i.height()+1.0)) <= 1.2) {
			emit found(i);
			return true;
		}
	}
	foreach(QFileInfo item, dirs) {
		if (DIRS.contains(item.fileName().toLower())) {
			if (_find(item.absoluteFilePath())) {
				return true;
			}
		}
	}
	emit found(_defaultCover);
	return false;
}

QImage &CoverFinder::defaultCover() {
	return _defaultCover;
}

bool CoverFinder::_extract(QString file) {
	QFileInfo info(file);
	QString suffix = info.suffix().toLower();
	TagLib::ID3v2::Tag *tag = NULL;
	bool todo = false;
	TagLib::File *f = NULL;
	if (suffix == "mp3") {
		f = new TagLib::MPEG::File(QFile::encodeName(file).data(), true, TagLib::AudioProperties::Fast);
		if (f == NULL) return false;
		tag = ((TagLib::MPEG::File*)f)->ID3v2Tag();
		todo = f->isValid();
	} else if (suffix == "flac") {
		f = new TagLib::FLAC::File(QFile::encodeName(file).data(), true, TagLib::AudioProperties::Fast);
		if (f == NULL) return false;
		tag = ((TagLib::FLAC::File*)f)->ID3v2Tag();
		todo = f->isValid();
	}
	if (todo && tag != NULL) {
		TagLib::ID3v2::FrameList l = tag->frameList("APIC");
		if (l.isEmpty())
			return false;
		TagLib::ID3v2::AttachedPictureFrame *pic = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(l.front());
		QImage img;
		img.loadFromData((const uchar *) pic->picture().data(), pic->picture().size());
		emit found(img);
		return true;
	}
	if (f != NULL) delete f;
	return false;
}

void CoverFinder::find(QFileInfo filePath) {
	QtConcurrent::run(this, &CoverFinder::_async_find, filePath);
}

bool CoverFinder::_async_find(QFileInfo filePath) {
	if (!_find(filePath.absolutePath()))
		return _extract(filePath.absoluteFilePath());
	return true;
}
