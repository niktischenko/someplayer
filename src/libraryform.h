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

#ifndef LIBRARYFORM_H
#define LIBRARYFORM_H

#include <QWidget>
#include "someplayer.h"

#include <QStandardItemModel>
#include <QStandardItem>
#include <QModelIndex>
#include "playlist.h"

namespace Ui {
    class LibraryForm;
}

using SomePlayer::DataObjects::Library;
using SomePlayer::DataObjects::Track;
using SomePlayer::DataObjects::Playlist;

enum LibraryFormListState {STATE_NONE, STATE_ARTIST, STATE_ALBUM, STATE_TRACK, STATE_PLAYLIST, STATE_PLAYLIST_TRACK, STATE_DYNAMIC};

class LibraryForm : public QWidget
{
    Q_OBJECT

public:
	explicit LibraryForm(Library *lib, QWidget *parent = 0);
    ~LibraryForm();
signals:
	void player(bool);
	void busy(QString);
	void done();
public slots:
	void search(QString &);
	void nextItem();
	void prevItem();
	void cancelSearch();
	void refresh();
	void updateIcons();
private slots:
	void _player();
	void _view_button();
	void _dynamic_button();
	void _playlists_button();
	void _toggle_select_all_button();
	void _add_button();
	void _delete_button();
	void _back_button();
	void _use_button();
	void _process_list_click(QModelIndex);

private:
    Ui::LibraryForm *ui;
	Library *_lib;
	QStandardItemModel *_model;
	LibraryFormListState _state;
	QString _current_artist;
	QString _current_album;
	Playlist _current_playlist;
	QList<Track> _current_tracks;
	QString _search_pattern;
	int _search_current_id;
	bool _current_playlist_changed;

	void _add_artist(QString artist);
	void _add_album(QString artist, QString album);
	void _add_track(Track track);
	void _add_playlist(QString name);
	void _delete_track(Track track);
	QString _icons_theme;
};

#endif // LIBRARYFORM_H
