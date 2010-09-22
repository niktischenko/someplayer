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

#include "playerform.h"
#include "ui_playerform.h"
#include "library.h"
#include "player/player.h"
#include <QDebug>
#include <QTime>
#include <QSlider>
#include "trackrenderer.h"
#include <QResource>

using namespace SomePlayer::DataObjects;
using namespace SomePlayer::Playback;

inline void __fill_list(QStandardItemModel *_model, Playlist playlist) {
	_model->clear();
	QList<Track> tracks = playlist.tracks();
	int count = tracks.count();
	_model->setRowCount(count);
	for (int i = 0; i < count; i++) {
		TrackMetadata meta = tracks.at(i).metadata();
		QString t = meta.title()+"#_#"+meta.artist()+"#_#"+meta.album();
		_model->setItem(i, 0, new QStandardItem(t));
	}
}

PlayerForm::PlayerForm(Library* lib, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerForm)
{
	_lib = lib;
	_player = new Player(this);
	_time = new QTime();
	ui->setupUi(this);
	if (_player->random()) {
		ui->randomButton->setIcon(QIcon(":/icons/random_active.png"));
	} else {
		ui->randomButton->setIcon(QIcon(":/icons/random_inactive.png"));
	}
	if (_player->repeat()) {
		ui->repeatButton->setIcon(QIcon(":/icons/repeat_active.png"));
	} else {
		ui->repeatButton->setIcon(QIcon(":/icons/repeat_inactive.png"));
	}
	_seek_slider = new QSlider(Qt::Horizontal);
	_seek_slider->setEnabled(false);
	ui->progressLayout->insertWidget(1, _seek_slider);
	_seek_slider->setTracking(false);
	_model = new QStandardItemModel(0, 2, this);
	ui->playlistView->setModel(_model);
	_context_menu = new QMenu(ui->playlistView);
	QAction *delete_action = _context_menu->addAction("Delete");
	QAction *enqueue_action = _context_menu->addAction("Enqueue");
	QAction *add_to_favorites = _context_menu->addAction("Add to favorites");

	_track_renderer = new TrackRenderer(this);
	ui->playlistView->setItemDelegateForColumn(0, _track_renderer);

	_tag_resolver = new TagResolver(this);

	connect(ui->libraryButton, SIGNAL(clicked()), this, SLOT(_library()));
	connect(ui->viewButton, SIGNAL(clicked()), this, SLOT(_toggle_view()));
	connect(ui->playlistView, SIGNAL(clicked(QModelIndex)), this, SLOT(_process_click(QModelIndex)));
	connect(ui->playpauseButton, SIGNAL(clicked()), _player, SLOT(toggle()));
	connect(ui->stopButton, SIGNAL(clicked()), _player, SLOT(stop()));
	connect(ui->nextButton, SIGNAL(clicked()), _player, SLOT(next()));
	connect(ui->prevButton, SIGNAL(clicked()), _player, SLOT(prev()));
	connect(_player, SIGNAL(trackChanged(Track)), this, SLOT(_track_changed(Track)));
	connect(_player, SIGNAL(tick(int,int)), this, SLOT(_tick(int,int)));
	connect(ui->randomButton, SIGNAL(clicked()), this, SLOT(_toggle_random()));
	connect(ui->repeatButton, SIGNAL(clicked()), this, SLOT(_toggle_repeat()));
	connect(_seek_slider, SIGNAL(sliderReleased()), this, SLOT(_slider_released()));
	connect(ui->playlistView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(_custom_context_venu_requested(QPoint)));
	connect(delete_action, SIGNAL(triggered()), this, SLOT(_delete_track()));
	connect(enqueue_action, SIGNAL(triggered()), this, SLOT(_enqueue_track()));
	connect(add_to_favorites, SIGNAL(triggered()), this, SLOT(_add_to_favorites()));
	connect(_player, SIGNAL(stateChanged(PlayerState)), this, SLOT(_state_changed(PlayerState)));
	connect(_player, SIGNAL(trackDone(Track)), _lib, SLOT(updateTrackCount(Track)));
	connect(_tag_resolver, SIGNAL(decoded(Track)), this, SLOT(_track_decoded(Track)));
}

PlayerForm::~PlayerForm()
{
    delete ui;
}

void PlayerForm::_library() {
	emit library();
}

void PlayerForm::reload() {
	if (ui->stackedWidget->currentIndex() == 1) {
		emit hideSearchPanel();
	}
	_current_playlist = _lib->getCurrentPlaylist();
	_player->setPlaylist(_current_playlist);
	__fill_list(_model, _current_playlist);
}

void PlayerForm::_toggle_view() {
	int index = ui->stackedWidget->currentIndex();
	index = (!index % 2);
	if (index) {
		ui->viewButton->setIcon(QIcon(":/icons/playlist.png"));
		emit hideSearchPanel();
	} else {
		ui->viewButton->setIcon(QIcon(":/icons/playback.png"));
		emit showSearchPanel();
	}
	ui->stackedWidget->setCurrentIndex(index);
}

void PlayerForm::_process_click(QModelIndex index) {
	int id = index.row();
	_player->stop();
	_player->setTrackId(id);
	_player->play();
	_track_renderer->setActiveRow(id);
	ui->playlistView->hide();
	ui->playlistView->show();
}

void PlayerForm::_track_changed(Track track) {
	int id = _current_playlist.tracks().indexOf(track);
	QModelIndex index = _model->index(id, 0);
	ui->playlistView->setCurrentIndex(index);
	ui->playlistView->scrollTo(index);
	_track_renderer->setActiveRow(id);
	ui->playlistView->hide();
	ui->playlistView->show();
	_display_track(track);
}

void PlayerForm::_display_track(Track track) {
	ui->countLabel->setText(QString("%1/%2").
							arg(_current_playlist.tracks().indexOf(track)+1).
							arg(_current_playlist.tracks().count()));
	ui->titleLabel->setText(QString("<h3>%1</h3>").arg(track.metadata().title()));
	ui->artistAlbumLabel->setText(QString("<b>%1</b><br/>%2").
								  arg(track.metadata().artist()).
								  arg(track.metadata().album()));
	_seek_slider->setMinimum(0);
	_seek_slider->setMaximum(track.metadata().length());
	_tick(0, track.metadata().length());
}

void PlayerForm::_tick(int done, int all) {
	_time->setHMS(0, all/60, all%60);
	ui->allTimeLabel->setText(_time->toString("mm:ss"));
	_time->setHMS(0, done/60, done%60);
	ui->doneTimeLabel->setText(_time->toString("mm:ss"));
	_seek_slider->setValue(done);
}

void PlayerForm::_slider_released() {
	_player->seek(_seek_slider->value());
}

void PlayerForm::_custom_context_venu_requested(const QPoint &pos) {
	_context_menu->exec(pos);
}

void PlayerForm::_delete_track() {
	QList<QModelIndex> idx = ui->playlistView->selectionModel()->selectedIndexes();
	int id = idx.first().row();
	_current_playlist.removeTrackAt(id);
	_lib->saveCurrentPlaylist(_current_playlist);
	reload();
}

void PlayerForm::_enqueue_track() {
	QList<QModelIndex> idx = ui->playlistView->selectionModel()->selectedIndexes();
	int id = idx.first().row();
	_player->enqueue(id);
}

void PlayerForm::_add_to_favorites() {
	QList<QModelIndex> idx = ui->playlistView->selectionModel()->selectedIndexes();
	int id = idx.first().row();
	_lib->addToFavorites(_current_playlist.tracks().at(id));
}

void PlayerForm::_state_changed(PlayerState state) {
	if (state == PLAYER_PLAYING) {
		ui->playpauseButton->setIcon(QIcon(":/icons/pause.png"));
		_seek_slider->setEnabled(true);
	} else {
		if (state == PLAYER_STOPPED) {
			_seek_slider->setValue(0);
			ui->doneTimeLabel->setText("00:00");
			_seek_slider->setEnabled(false);
		}
		ui->playpauseButton->setIcon(QIcon(":/icons/play.png"));
	}
}

void PlayerForm::_toggle_random() {
	_player->toggleRandom();
	if (_player->random()) {
		ui->randomButton->setIcon(QIcon(":/icons/random_active.png"));
	} else {
		ui->randomButton->setIcon(QIcon(":/icons/random_inactive.png"));
	}
}

void PlayerForm::_toggle_repeat() {
	_player->toggleRepeat();
	if (_player->repeat()) {
		ui->repeatButton->setIcon(QIcon(":/icons/repeat_active.png"));
	} else {
		ui->repeatButton->setIcon(QIcon(":/icons/repeat_inactive.png"));
	}
}

void PlayerForm::search(QString &pattern) {
	_search_pattern = pattern;
	_search_current_id = -1;
	nextItem();
}

void PlayerForm::nextItem() {
	QString data = _model->index(_search_current_id, 0).data().toString();
	for (int i = _search_current_id+1; i < _model->rowCount(); i++) {
		data = _model->index(i, 0).data().toString();
		if (data.contains(_search_pattern, Qt::CaseInsensitive)) {
			_search_current_id = i;
			break;
		}
	}
	QModelIndex id = _model->index(_search_current_id, 0);
	_track_renderer->setSearchRow(_search_current_id);
	ui->playlistView->scrollTo(id);
	ui->playlistView->hide();
	ui->playlistView->show();
}

void PlayerForm::prevItem() {
	QString data = _model->index(_search_current_id, 0).data().toString();
	for (int i = _search_current_id-1; i >= 0; i--) {
		data = _model->index(i, 0).data().toString();
		if (data.contains(_search_pattern, Qt::CaseInsensitive)) {
			_search_current_id = i;
			break;
		}
	}
	QModelIndex id = _model->index(_search_current_id, 0);
	_track_renderer->setSearchRow(_search_current_id);
	ui->playlistView->scrollTo(id);
	ui->playlistView->hide();
	ui->playlistView->show();
}

void PlayerForm::cancelSearch() {
	_search_pattern = "";
	_track_renderer->setSearchRow(-1);
	ui->playlistView->scrollTo(_model->index(_track_renderer->activeRow(), 0));
	ui->playlistView->hide();
	ui->playlistView->show();
}

void PlayerForm::addFiles(QList<QString> files) {
	_tag_resolver->decode(files);
}

void PlayerForm::_track_decoded(Track track) {
	_current_playlist.addTrack(track);
	__fill_list(_model, _current_playlist);
	_lib->saveCurrentPlaylist(_current_playlist);
	_player->setPlaylist(_current_playlist);
}
