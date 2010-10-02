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

#include "libraryform.h"
#include "ui_libraryform.h"
#include "library.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QModelIndex>
#include <QModelIndexList>
#include "track.h"
#include "playlist.h"
#include <QTime>
#include <QQueue>
#include <QMessageBox>
#include "config.h"

using namespace SomePlayer::DataObjects;
using namespace SomePlayer::Storage;

inline QString __format_track_string(TrackMetadata meta) {
	int minutes = meta.length() / 60;
	int seconds = meta.length() % 60;
	QTime time(0, minutes, seconds);
	return QString("[%1] %2").arg(time.toString("mm:ss")).arg(meta.title());

}

inline void __fill_model(QStandardItemModel *model, QList<QString> data) {
	model->clear();
	int count = data.count();
	model->setRowCount(count);
	for (int i = 0; i < count; i++) {
		model->setItem(i, 0, new QStandardItem(data.at(i)));
	}
}

inline void __fill_model_tracks (QStandardItemModel *model, QList<Track> tracks) {
	int count = tracks.count();
	model->setRowCount(count);
	Config config;
	bool show_lenght = (config.getValue("ui/showtracklenght").toString() != "no");
	for (int i = 0; i < count; i++) {
		TrackMetadata meta = tracks.at(i).metadata();
		if (show_lenght)
			model->setItem(i, 0, new QStandardItem(__format_track_string(meta)));
		else
			model->setItem(i, 0, new QStandardItem(meta.title()));
	}
}

LibraryForm::LibraryForm(Library *lib, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LibraryForm)
{
	_lib = lib;
	_model = new QStandardItemModel(this);
	_state = STATE_NONE;
	ui->setupUi(this);
	connect(ui->playerButton, SIGNAL(clicked()), this, SLOT(_player()));
	connect(ui->viewButton, SIGNAL(clicked()), this, SLOT(_view_button()));
	connect(ui->playlistsButton, SIGNAL(clicked()), this, SLOT(_playlists_button()));
	connect(ui->dynamicButton, SIGNAL(clicked()), this, SLOT(_dynamic_button()));
	connect(ui->listView, SIGNAL(clicked(QModelIndex)), this, SLOT(_process_list_click(QModelIndex)));
	connect(ui->addButton, SIGNAL(clicked()), this, SLOT(_add_button()));
	connect(ui->selectAllButton, SIGNAL(clicked()), this, SLOT(_toggle_select_all_button()));
	connect(ui->backButton, SIGNAL(clicked()), this, SLOT(_back_button()));
	connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(_delete_button()));
	connect(ui->useButton, SIGNAL(clicked()), this, SLOT(_use_button()));
	_view_button();
	Config config;
	_icons_theme = config.getValue("ui/iconstheme").toString();
	_current_playlist_changed = true;
}

LibraryForm::~LibraryForm()
{
	_lib->saveCurrentPlaylist(_lib->getCurrentPlaylist()); // wtf?
	_current_playlist_changed = true;
	delete ui;
}

void LibraryForm::_player() {
	emit player(_current_playlist_changed);
	_current_playlist_changed = false;
}

void LibraryForm::_view_button() {
	QList<QString> artitst = _lib->getArtists();
	__fill_model(_model, artitst);
	ui->listView->setModel(_model);
	_state = STATE_ARTIST;
	ui->backButton->hide();
	ui->listLabel->setText("Artists");
	ui->addButton->show();
	ui->deleteButton->hide();
	ui->useButton->hide();
}

void LibraryForm::_dynamic_button() {
	ui->useButton->hide();
	ui->backButton->hide();
	ui->addButton->show();
	ui->deleteButton->hide();
	_model->clear();
	_model->setRowCount(4);
	_model->setItem(0, new QStandardItem("Favorites"));
	_model->setItem(1, new QStandardItem("Most played"));
	_model->setItem(2, new QStandardItem("Never played"));
	_model->setItem(3, new QStandardItem("Recently added"));
	_state = STATE_DYNAMIC;
}

void LibraryForm::_process_list_click(QModelIndex index) {
	if (_state == STATE_NONE) return;
	QString data = index.data().toString();
	switch (_state) {
	case STATE_ARTIST:
		__fill_model(_model, _lib->getAlbumsForArtist(data));
		ui->listView->scrollToTop();
		_current_artist = data;
		_state = STATE_ALBUM;
		ui->backButton->show();
		ui->listLabel->setText(QString("Albums by \"%1\"").arg(_current_artist));
		break;
	case STATE_ALBUM:
		_current_album = data;
		_current_tracks = _lib->getTracksForAlbum(data, _current_artist);
		__fill_model_tracks(_model, _current_tracks);
		ui->listView->scrollToTop();
		_state = STATE_TRACK;
		ui->backButton->show();
		ui->listLabel->setText(QString("Tracks from \"%1\" by \"%2\"").arg(_current_album).arg(_current_artist));
		break;
	case STATE_PLAYLIST:
		{
			_current_playlist = _lib->getPlaylist(data);
			_current_tracks = _current_playlist.tracks();
			__fill_model_tracks(_model, _current_tracks);
			ui->listView->scrollToTop();
			_state = STATE_PLAYLIST_TRACK;
			ui->backButton->show();
			ui->deleteButton->show();
			ui->useButton->show();
			ui->listLabel->setText(QString("Tracks in playlist \"%1\"").arg(data));
		}
		break;
	case STATE_DYNAMIC:
		{
			switch(index.row()) {
			case 0: //favorites
				_current_playlist = _lib->getFavorites();
				break;
			case 1: //most played
				_current_playlist = _lib->getMostPlayed();
				break;
			case 2: //never played
				_current_playlist = _lib->getNeverPlayed();
			case 3: //recently added
				_current_playlist = _lib->getRecentlyAdded();
				break;
			default:
				return;
			}
			_current_tracks = _current_playlist.tracks();
			__fill_model_tracks(_model, _current_tracks);
			ui->listView->scrollToTop();
			_state = STATE_PLAYLIST_TRACK;
			ui->backButton->show();
			ui->useButton->show();
			ui->addButton->show();
			ui->listLabel->setText(_current_playlist.name());
		}
	default:
		return;
	}
}

void LibraryForm::_add_button() {
	if (_state == STATE_NONE) return;
	QModelIndexList selected = ui->listView->selectionModel()->selectedIndexes();
	ui->listView->selectionModel()->clearSelection();
	emit busy(QString("<H1>Adding... Please wait</H1>"));
	switch (_state) {
	case STATE_ARTIST:
		foreach (QModelIndex id, selected) {
			_add_artist(id.data().toString());
		}
		break;
	case STATE_ALBUM:
		foreach (QModelIndex id, selected) {
			_add_album(_current_artist, id.data().toString());
		}
		break;
	case STATE_TRACK:
		foreach (QModelIndex id, selected) {
			_add_track(_current_tracks.at(id.row()));
		}
		break;
	case STATE_PLAYLIST:
		foreach (QModelIndex id, selected) {
			_add_playlist(id.data().toString());
		}
		break;
	case STATE_PLAYLIST_TRACK:
		foreach (QModelIndex id, selected) {
			_add_track(_current_tracks.at(id.row()));
		}
		break;
	default:
		emit done();
		return;
	}
	emit done();
}


void LibraryForm::_add_artist(QString artist) {
	QList<QString> albums = _lib->getAlbumsForArtist(artist);
	foreach(QString album, albums) {
		_add_album(artist, album);
	}
}

void LibraryForm::_add_album(QString artist, QString album) {
	QList<Track> tracks = _lib->getTracksForAlbum(album, artist);
	foreach(Track track, tracks) {
		_add_track(track);
	}
}

void LibraryForm::_add_track(Track track) {
	Playlist current = _lib->getCurrentPlaylist();
	current.addTrack(track);
	_lib->saveCurrentPlaylist(current);
	_current_playlist_changed = true;
}

void LibraryForm::_add_playlist(QString name) {
	Playlist playlist = _lib->getPlaylist(name);
	QList<Track> tracks = playlist.tracks();
	foreach (Track track, tracks) {
		_add_track(track);
	}
}

void LibraryForm::_back_button() {
	switch (_state) {
	case STATE_ALBUM:
		_view_button();
		ui->listView->scrollToTop();
		break;
	case STATE_TRACK:
		__fill_model(_model, _lib->getAlbumsForArtist(_current_artist));
		ui->listView->scrollToTop();
		_state = STATE_ALBUM;
		ui->listLabel->setText(QString("Albums by \"%1\"").arg(_current_artist));
		break;
	case STATE_PLAYLIST_TRACK:
		_playlists_button();
		ui->listView->scrollToTop();
	default:
		return;
	}
}

void LibraryForm::_playlists_button() {
	QList<QString> playlists = _lib->getPlaylistsNames();
	__fill_model(_model, playlists);
	ui->listView->setModel(_model);
	_state = STATE_PLAYLIST;
	ui->backButton->hide();
	ui->listLabel->setText("Playlists");
	ui->addButton->hide();
	ui->deleteButton->show();
	ui->useButton->hide();
}

void LibraryForm::_delete_button() {
	if (_state == STATE_PLAYLIST_TRACK) {
		QModelIndexList selected = ui->listView->selectionModel()->selectedIndexes();
		ui->listView->selectionModel()->clearSelection();
		QQueue<int> to_delete;
		foreach (QModelIndex id, selected) {
			to_delete.append(id.row());
		}
		qSort(to_delete);
		int count = to_delete.count();
		for (int i = count-1; i >= 0; i--) {
			_current_playlist.removeTrackAt(to_delete.at(i));
		}
		_current_tracks = _current_playlist.tracks();
		_lib->savePlaylist(_current_playlist);
		__fill_model_tracks(_model, _current_tracks);
	} else if (_state == STATE_PLAYLIST) {
		QModelIndexList selected = ui->listView->selectionModel()->selectedIndexes();
		QQueue<int> to_delete;
		foreach (QModelIndex id, selected) {
			to_delete.append(id.row());
		}
		qSort(to_delete);
		int count = to_delete.count();
		for (int i = count-1; i >= 0; i--) {
			QString name = _model->item(to_delete.at(i))->text();
			if (name != _CURRENT_PLAYLIST_SUBST_) {
				_lib->removePlaylist(name);
				_model->removeRow(to_delete.at(i));
			}
		}
	}
}

void LibraryForm::_delete_track(Track track) {
	Playlist current = _lib->getCurrentPlaylist();
	current.removeTrack(track);
	_lib->saveCurrentPlaylist(current);
	_current_playlist_changed = true;
}

void LibraryForm::_use_button() {
	_lib->saveCurrentPlaylist(_current_playlist);
	_current_playlist_changed = true;
	_current_playlist = _lib->getCurrentPlaylist();
}

void LibraryForm::search(QString &pattern) {
	_search_pattern = pattern;
	_search_current_id = -1;
	nextItem();
}

void LibraryForm::nextItem() {
	QString data = _model->index(_search_current_id, 0).data().toString();
	for (int i = _search_current_id+1; i < _model->rowCount(); i++) {
		data = _model->index(i, 0).data().toString();
		if (data.contains(_search_pattern, Qt::CaseInsensitive)) {
			_search_current_id = i;
			break;
		}
	}
	QModelIndex id = _model->index(_search_current_id, 0);
	ui->listView->selectionModel()->clearSelection();
	ui->listView->selectionModel()->select(id, QItemSelectionModel::Select);
	ui->listView->scrollTo(id);
}

void LibraryForm::prevItem() {
	QString data = _model->index(_search_current_id, 0).data().toString();
	for (int i = _search_current_id-1; i >= 0; i--) {
		data = _model->index(i, 0).data().toString();
		if (data.contains(_search_pattern, Qt::CaseInsensitive)) {
			_search_current_id = i;
			break;
		}
	}
	QModelIndex id = _model->index(_search_current_id, 0);
	ui->listView->selectionModel()->clearSelection();
	ui->listView->selectionModel()->select(id, QItemSelectionModel::Select);
	ui->listView->scrollTo(id);
}

void LibraryForm::cancelSearch() {
	_search_pattern = "";
	ui->listView->selectionModel()->clearSelection();
}

void LibraryForm::refresh() {
	switch (_state) {
	case STATE_ARTIST:
		_view_button();
		break;
	case STATE_ALBUM:
		__fill_model(_model, _lib->getAlbumsForArtist(_current_artist));
		break;
	case STATE_PLAYLIST:
		_playlists_button();
		break;
	case STATE_DYNAMIC:
		_dynamic_button();
		break;
	case STATE_PLAYLIST_TRACK:
		_current_playlist = _lib->getPlaylist(_current_playlist.name());
		_current_tracks = _current_playlist.tracks();
		__fill_model_tracks(_model, _current_tracks);
		break;
	case STATE_TRACK:
		_current_tracks = _lib->getTracksForAlbum(_current_album, _current_artist);
		__fill_model_tracks(_model, _current_tracks);
		break;
	default:
		return;
	}
}

void LibraryForm::_toggle_select_all_button() {
	if (ui->listView->selectionModel()->selectedIndexes().count() == ui->listView->model()->rowCount()) {
		ui->listView->selectionModel()->clearSelection();
		ui->selectAllButton->setIcon(QIcon(":/icons/"+_icons_theme+"/select_all.png"));
	} else {
		ui->listView->selectAll();
		ui->selectAllButton->setIcon(QIcon(":/icons/"+_icons_theme+"/deselect_all.png"));
	}
}

void LibraryForm::updateIcons() {
	Config config;
	_icons_theme = config.getValue("ui/iconstheme").toString();
	ui->backButton->setIcon(QIcon(":/icons/"+_icons_theme+"/back.png"));
	if (ui->listView->selectionModel()->selectedIndexes().count() == ui->listView->model()->rowCount())
		ui->selectAllButton->setIcon(QIcon(":/icons/"+_icons_theme+"/deselect_all.png"));
	else
		ui->selectAllButton->setIcon(QIcon(":/icons/"+_icons_theme+"/select_all.png"));
	ui->addButton->setIcon(QIcon(":/icons/"+_icons_theme+"/add.png"));
	ui->deleteButton->setIcon(QIcon(":/icons/"+_icons_theme+"/delete.png"));
	ui->useButton->setIcon(QIcon(":/icons/"+_icons_theme+"/use.png"));
	ui->playerButton->setIcon(QIcon(":/icons/"+_icons_theme+"/player.png"));
	ui->viewButton->setIcon(QIcon(":/icons/"+_icons_theme+"/artists.png"));
	ui->dynamicButton->setIcon(QIcon(":/icons/"+_icons_theme+"/dynamic.png"));
	ui->playlistsButton->setIcon(QIcon(":/icons/"+_icons_theme+"/playlists.png"));
}
