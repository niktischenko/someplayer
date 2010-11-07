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

#ifndef COVERFINDER_H
#define COVERFINDER_H

#include <QObject>
#include <QImage>
#include "someplayer.h"

class CoverFinder : public QObject
{
	Q_OBJECT
public:
	explicit CoverFinder(QObject *parent = 0);

signals:
	void found(QImage);

public slots:
	bool find(QString path);
	bool extract(QString file);
	QImage &defaultCover();

private:
	QImage _defaultCover;
	QStringList SUFFIXES;
	QStringList NAMES;
	QStringList DIRS;

};

#endif // COVERFINDER_H
