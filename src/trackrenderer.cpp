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
#include <QStandardItem>
#include <QApplication>
#include <QDesktopWidget>

using namespace SomePlayer::Storage;
TrackRenderer::TrackRenderer(QObject *parent) :
    AbstractItemRenderer(parent)
{
	Config config;
	_icons_theme = config.getValue("ui/iconstheme").toString();
	_apen = QPen(QColor::fromRgb(255, 255, 255, 128));
	_spen = QPen(QColor::fromRgb(100, 150, 220));
	_sspen = QPen(QColor::fromRgb(100, 220, 150));

}

void TrackRenderer::updateIcons() {
	Config config;
	_icons_theme = config.getValue("ui/iconstheme").toString();
}

void TrackRenderer::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
	if (index.column() == 0) {
		int x1, y1, x2, y2;
		option.rect.getCoords(&x1, &y1, &x2, &y2);
		QImage image(":/icons/"+_icons_theme+"/arrow_r.png");
		x1 += (x2 - x1 - image.width())/2;
		y1 += (y2 - y1 - image.width())/2;
		painter->drawImage(x1, y1, image);
	} else {
		QString value = index.data().toString();
		QStringList meta = value.split("#_#");

		QFont f = painter->font();
		QFont fp = painter->font();

		int x1, y1, x2, y2;
		option.rect.getCoords(&x1, &y1, &x2, &y2);

		QPen pen = painter->pen();
		f.setBold(false);
		if (index.row() == _search_row) {
			f.setBold(true);
			painter->setPen(_sspen);
		} else if (index.row() == _active_row) {
			f.setBold(true);
			painter->setPen(_spen);
		} else {
			painter->setPen(pen);
		}
		painter->setFont(f);
		painter->drawText(x1+10, y1 + 1*(y2-y1)/2, meta[0]);
		fp.setBold(false);
		fp.setPointSize(f.pointSize()*3/4);
		painter->setFont(fp);
		painter->setPen(_apen);
		painter->drawText(x1+10, y1 + 3*(y2-y1)/5, (x2-x1)-100, 2*fp.pointSize(), Qt::AlignAbsolute, QString("%1 (%2)").arg(meta[2]).arg(meta[1]));
		painter->drawText(x2-60, y1 + 3*(y2-y1)/5, 55, 2*fp.pointSize(), Qt::AlignAbsolute, QString("%1").arg(meta[3]));
		painter->setFont(f);
		painter->setPen(pen);
	}
}

QSize TrackRenderer::sizeHint(const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const {
	return QSize(QApplication::desktop()->geometry().width(), 80);
}

void TrackRenderer::setActiveTrackColor(QString color) {
	QColor c;
	if (color == "blue") {
		c = QColor::fromRgb(100, 150, 220);
	} else if (color == "black") {
		c = QColor::fromRgb(0, 0, 0);
	} else if (color == "magenta") {
		c = QColor::fromRgb(150, 80, 150);
	} else if (color == "red") {
		c = QColor::fromRgb(220, 100, 100);
	} else if (color == "yellow") {
		c = QColor::fromRgb(220, 220, 100);
	} else if (color == "white") {
		c = QColor::fromRgb(255, 255, 255);
	} else if (color == "dark") {
		c = QColor::fromRgb(70, 70, 70);
	} else if (color == "light") {
		c = QColor::fromRgb(200, 200, 200);
	}
	_spen = QPen(c);
}
