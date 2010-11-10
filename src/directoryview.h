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

#ifndef DIRECTORYVIEW_H
#define DIRECTORYVIEW_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QItemSelection>
#include "tagresolver.h"
#include "mediascanner.h"

namespace Ui {
	class DirectoryView;
}

using SomePlayer::DataObjects::TagResolver;
using SomePlayer::Storage::MediaScanner;
using SomePlayer::DataObjects::Track;

class DirectoryView : public QWidget
{
	Q_OBJECT

public:
	explicit DirectoryView(QWidget *parent = 0);
	~DirectoryView();
	void updateIcons();
	void updateGradient();
	void lanscapeMode();
	void portraitMode();
	void updateTranslations();

signals:
	void addAndPlay(Track);
	void addTracks(QList<Track>);

private:
	Ui::DirectoryView *ui;
	QStandardItemModel *_model;
	QList<QString> REGISTERED_FILE_EXTENSIONS;
	void readDir(QString path);
	QList<QString> _directories;
	QList<QString> _files;
	QString _icons_theme;
	QString _current_dir;
	TagResolver *_tagresolver;
	MediaScanner *_mediascanner;
	QList<Track> _tracks;
	QString _bottom_gradient;
	QString _top_gradient;

private slots:
	void _back();
	void _home();
	void _toggle_selection();
	void _add();
	void _add_track(Track);
	void _done();
	void _process_click(QModelIndex);
	void _process_dblclick(QModelIndex);
	void _process_selection(QItemSelection, QItemSelection);

};

#endif // DIRECTORYVIEW_H
