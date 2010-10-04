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

#ifndef ABSTRACTITEMRENDERER_H
#define ABSTRACTITEMRENDERER_H

#include <QAbstractItemDelegate>
#include <QPainter>

class AbstractItemRenderer : public QAbstractItemDelegate
{
	Q_OBJECT
public:
	explicit AbstractItemRenderer(QObject *parent = 0) : QAbstractItemDelegate(parent) {}

signals:

public slots:
	void setActiveRow(int r) {_active_row = r;}
	void setSearchRow(int r) {_search_row = r;}
	int activeRow() {return _active_row;}
	int searchRow() {return _search_row;}
protected:
	int _active_row;
	int _search_row;

};

#endif // ABSTRACTITEMRENDERER_H
