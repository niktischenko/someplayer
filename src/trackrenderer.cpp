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

#include "trackrenderer.h"
#include <QFont>
#include <QFontMetrics>
#include <QSize>
#include <QColor>

TrackRenderer::TrackRenderer(QObject *parent) :
    QAbstractItemDelegate(parent)
{
}

void TrackRenderer::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
	QString value = index.data().toString();
	QStringList meta = value.split("#_#");

	QFont f = painter->font();
	QFont fp = painter->font();

	int x1, y1, x2, y2;
	option.rect.getCoords(&x1, &y1, &x2, &y2);

	QPen pen = painter->pen();
	QPen npen (QColor::fromRgb(80, 130, 255, 50));
	QPen apen (QColor::fromRgb(255, 255, 255, 128));
	QPen spen (QColor::fromRgb(100, 150, 220));
	QPen sspen (QColor::fromRgb(100, 220, 150));

	f.setBold(true);
	painter->setFont(f);
	f.setBold(false);
	painter->setPen(npen);
	painter->drawLine(x1, y1, x2, y1);
	if (index.row() == _search_row) {
		painter->setPen(sspen);
	} else if (index.row() == _active_row) {
		painter->setPen(spen);
	} else {
		painter->setPen(pen);
	}
	painter->drawText(x1, y1 + 2*(y2-y1)/5, meta[0]);
	fp.setBold(false);
	fp.setPointSize(f.pointSize()*3/4);
	painter->setFont(fp);
	painter->setPen(apen);
	painter->drawText((x2+x1)/2, y1 + 4*(y2-y1)/6, meta[1]);
	painter->drawText((x2+x1)/2, y1 + 11*(y2-y1)/12, meta[2]);
	painter->setPen(npen);
	painter->drawLine(x1, y2, x2, y2);
	painter->setFont(f);
	painter->setPen(pen);
}

QSize TrackRenderer::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const {
	return QSize(option.rect.width(), 80);
}

void TrackRenderer::setActiveRow(int r) {
	_active_row = r;
}

void TrackRenderer::setSearchRow(int r) {
	_search_row = r;
}
