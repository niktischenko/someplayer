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

#ifndef PLAYLISTSORTFORM_H
#define PLAYLISTSORTFORM_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QItemSelection>
#include <QItemSelectionModel>
#include "someplayer.h"
#include "playlist.h"
#include "trackrenderer.h"

using SomePlayer::DataObjects::Playlist;

namespace Ui {
	class PlaylistSortForm;
}

class PlaylistSortForm : public QWidget
{
	Q_OBJECT

public:
	explicit PlaylistSortForm(QWidget *parent = 0);
	~PlaylistSortForm();
	void setPlaylist(Playlist playlist);
	Playlist getPlaylist();

private slots:
	void done();
	void updateView();
	void _process_selection(QItemSelection,QItemSelection);
	void moveUp();
	void moveDown();
	void sortAlphAsc();
	void sortAlphDesc();
	void sortArtist();
	void shuffle();

public slots:
	void landscapeMode();
	void portraitMode();
	void updateTranslations();
	void updateGradiend();
	void updateIcons();

signals:
	void playlistChanged();

private:
	Ui::PlaylistSortForm *ui;
	Playlist _playlist;
	TrackRenderer *_track_renderer;
	QStandardItemModel *_model;
	void _move(QList<int> rows, int diff);
	QString _bottom_gradient;
	QString _top_gradient;
};

#endif // PLAYLISTSORTFORM_H
