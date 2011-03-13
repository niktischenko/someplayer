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

using namespace SomePlayer::DataObjects;

CoverFinder::CoverFinder(QObject *parent) :
		QObject(parent)
{
	_defaultCover = QImage(":/images/defaultcover.png");
	SUFFIXES << "png" << "jpg" << "jpeg" << "bmp" << "gif";
	NAMES << "cover" << "folder" << "album";
	DIRS << "cover" << "folder" << ".cover" << ".folder";
	PRIOR_NAMES << "front" << "cover";
	UNPRIOR_NAMES << "disk" << "back" << "booklet" << "cd";
}

bool CoverFinder::_find(QString path) {
	QDir dir(path);
	QFileInfoList items = dir.entryInfoList(QDir::Files);
	QFileInfoList dirs = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Hidden);
	QFileInfoList pics;
	QFileInfoList appropriate_pics;
	foreach (QFileInfo item, items) {
		if (SUFFIXES.contains(item.suffix().toLower())) {
			pics << item;
			if (NAMES.contains(item.baseName().toLower())) {
				emit found(QImage(item.absoluteFilePath()));
				emit foundPath(item.absoluteFilePath());
				return true;
			}
		}
	}
	foreach (QFileInfo item, pics) {
		QImage i(item.absoluteFilePath());
		if (0.8 <= (i.width()*1.0/(i.height()+1.0)) <= 1.2) {
			appropriate_pics << item;
		}
	}
	if (!appropriate_pics.isEmpty()) {
		foreach (QString name, PRIOR_NAMES) {
			foreach (QFileInfo item, appropriate_pics) {
				if (item.baseName().toLower().contains(name)) {
					emit found(QImage(item.absoluteFilePath()));
					emit foundPath(item.absoluteFilePath());
					return true;
				}
			}
		}
		QFileInfoList unprior;
		foreach (QString name, UNPRIOR_NAMES) {
			foreach (QFileInfo item, appropriate_pics) {
				if (item.baseName().toLower().contains(name)) {
					unprior << item;
				}
			}
		}
		if (appropriate_pics.size() > unprior.size()) {
			foreach (QFileInfo item, appropriate_pics) {
				if (!unprior.contains(item)) {
					emit found(QImage(item.absoluteFilePath()));
					emit foundPath(item.absoluteFilePath());
					return true;
				}
			}
		}
		emit found(QImage(unprior.at(0).absoluteFilePath()));
		emit foundPath(unprior.at(0).absoluteFilePath());
		return true;
	}
	foreach(QFileInfo item, dirs) {
		if (DIRS.contains(item.fileName().toLower())) {
			if (_find(item.absoluteFilePath())) {
				return true;
			}
		}
	}
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
		emit foundPath("");
		return true;
	}
	if (f != NULL) delete f;
	return false;
}

void CoverFinder::find(Track track) {
	QFileInfo filePath(track.source());
	QtConcurrent::run(this, &CoverFinder::_async_find, filePath, track);
}

bool CoverFinder::_async_find(QFileInfo filePath, Track track) {
	if (!_find(filePath.absolutePath()) && !_tfind(track.metadata().artist(), track.metadata().album()) &&
	    !_malfind(track) && !_extract(filePath.absoluteFilePath())) {
		emit found(_defaultCover);
		emit foundPath("");
		return false;
	}
	return true;
}

bool CoverFinder::_tfind(QString artist, QString album) {
	QString aname = artist.toLower();
	QString aalbum = album.toLower();
	aname.replace("/", "");
	QString aaname = aname+" - "+aalbum;
	aaname.replace("/", "");
	QString fname1 = QDir::homePath()+"/.covers/"+aaname+".jpg";
	QString fname2 = QDir::homePath()+"/.covers/"+aname+".jpg";
	if (QFile::exists(fname1)) {
		emit found(QImage(fname1));
		emit foundPath(fname1);
		return true;
	} else if (QFile::exists(fname2)) {
		emit found(QImage(fname2));
		emit foundPath(fname2);
		return true;
	}
	return false;
}

bool CoverFinder::_malfind(Track track) {
	QString path = QFileInfo(track.source()).absolutePath()+"/.mediaartlocal/"+track.mediaArtLocal();
	if (QFile::exists(path)) {
		emit found(QImage(path));
		emit foundPath(path);
		return true;
	}
	path = QDir::homePath()+"/.cache/media-art/"+track.mediaArtLocal();
	if (QFile::exists(path)) {
		emit found(QImage(path));
		emit foundPath(path);
		return true;
	}
	return false;
}
