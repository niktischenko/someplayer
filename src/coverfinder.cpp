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
#include <mpegfile.h>
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

bool CoverFinder::find(QString path) {
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
			if (find(item.absoluteFilePath())) {
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

bool CoverFinder::extract(QString file) {
	QFileInfo info(file);
	QString suffix = info.suffix().toLower();
	TagLib::ID3v2::Tag *tag = NULL;
	if (suffix == "mp3") {
		TagLib::MPEG::File f(QFile::encodeName(file).data());
		tag = f.ID3v2Tag();
		if (f.isValid() && tag != NULL) {
			TagLib::ID3v2::FrameList l = tag->frameList("APIC");
			if (l.isEmpty())
				return false;
			TagLib::ID3v2::AttachedPictureFrame *pic = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(l.front());
			QImage img;
			img.loadFromData((const uchar *) pic->picture().data(), pic->picture().size());
			emit found(img);
			return true;
		}
	}
	return false;
}
