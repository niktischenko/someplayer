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

#ifndef PLAYERFORM_H
#define PLAYERFORM_H

#include <QWidget>
#include "someplayer.h"
#include "playlist.h"
#include <QStandardItemModel>
#include "player/player.h"
#include <QSlider>
#include <QMenu>
#include <QTime>
#include "trackrenderer.h"
#include "tagresolver.h"

namespace Ui {
    class PlayerForm;
}

using SomePlayer::DataObjects::Library;
using SomePlayer::DataObjects::Playlist;
using SomePlayer::DataObjects::Track;
using SomePlayer::Playback::Player;
using SomePlayer::Playback::PlayerState;
using SomePlayer::DataObjects::TagResolver;

class PlayerForm : public QWidget
{
    Q_OBJECT

public:
	explicit PlayerForm(Library *lib, QWidget *parent = 0);
    ~PlayerForm();
signals:
	void library();
	void showSearchPanel();
	void hideSearchPanel();

public slots:
	void reload();
	void search(QString &);
	void nextItem();
	void prevItem();
	void cancelSearch();
	void addFiles(QList<QString> files);
	void stop();

private slots:
	void _library();
	void _toggle_view();
	void _process_click(QModelIndex);
	void _track_changed(Track);
	void _tick(int, int);
	void _slider_released();
	void _custom_context_venu_requested ( const QPoint & pos );
	void _delete_track();
	void _enqueue_track();
	void _add_to_favorites();
	void _add_to_playlists();
	void _edit_tags();
	void _state_changed(PlayerState);
	void _toggle_repeat();
	void _toggle_random();
	void _toggle_volume();
	void _track_decoded(Track);
	void _volume_changed();

private:
    Ui::PlayerForm *ui;
	Library *_lib;
	Playlist _current_playlist;
	QTime *_time;
	QStandardItemModel *_model;
	Player *_player;
	QSlider *_seek_slider;
	QMenu *_context_menu;
	QString _search_pattern;

	TrackRenderer *_track_renderer;
	TagResolver *_tag_resolver;
	void _display_track(Track);
	int _search_current_id;
};

#endif // PLAYERFORM_H
