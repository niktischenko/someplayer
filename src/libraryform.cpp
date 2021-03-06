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
#include <QTimer>
#include <QQueue>
#include <QMessageBox>
#include "config.h"
#include <QSpacerItem>
#include <QDebug>

using namespace SomePlayer::DataObjects;
using namespace SomePlayer::Storage;

inline QString __format_track_string(TrackMetadata meta) {
	int minutes = meta.length() / 60;
	int seconds = meta.length() % 60;
	QTime time(0, minutes, seconds);
	return QString("[%1] %2").arg(time.toString("mm:ss")).arg(meta.title());

}

inline void __fill_model(QStandardItemModel *model, QList<QString> data, QString icons_theme) {
	model->clear();
	int count = data.count();
	model->setRowCount(count);
	for (int i = 0; i < count; i++) {
		model->setItem(i, 0, new QStandardItem(QIcon(":/icons/"+icons_theme+"/deselect_all.png"), ""));
		model->setItem(i, 1, new QStandardItem(data.at(i)));
	}
}

inline void __fill_model_album(QStandardItemModel *model, QMap<QString, int> data, QString icons_theme) {
	model->clear();
	int count = data.count();
	model->setRowCount(count+1);
	int i = 0;
	Config config;
	model->setItem(i, 0, new QStandardItem(QIcon(":/icons/"+icons_theme+"/deselect_all.png"), ""));
	model->setItem(i, 1, new QStandardItem(QObject::tr("All tracks")));
	i++;
	if (config.getValue("ui/albumsorting").toString() == "date") {
		QMap<int, QList<QString> > years;
		foreach (QString name, data.keys()) {
			years[data[name]].append(name);
		}
		QList<int> keys = years.keys();
		qSort(keys);

		foreach (int year, keys) {
			foreach (QString name, years[year]) {
				model->setItem(i, 0, new QStandardItem(QIcon(":/icons/"+icons_theme+"/deselect_all.png"), ""));
				model->setItem(i, 1, new QStandardItem(QString("[%1] %2").arg(year).arg(name)));
				i++;
			}
		}
	} else {
		QList<QString> names = data.keys();
		foreach (QString name, names) {
			model->setItem(i, 0, new QStandardItem(QIcon(":/icons/"+icons_theme+"/deselect_all.png"), ""));
			model->setItem(i, 1, new QStandardItem(QString("[%1] %2").arg(data[name]).arg(name)));
			i++;
		}
	}
}

inline void __fill_model_tracks (QStandardItemModel *model, QList<Track> tracks, QString icons_theme) {
	int count = tracks.count();
	model->setRowCount(count);
	Config config;
	bool show_lenght = (config.getValue("ui/showtracklenght").toString() != "no");
	for (int i = 0; i < count; i++) {
		TrackMetadata meta = tracks.at(i).metadata();
		if (show_lenght) {
			model->setItem(i, 0, new QStandardItem(QIcon(":/icons/"+icons_theme+"/deselect_all.png"), ""));
			model->setItem(i, 1, new QStandardItem(__format_track_string(meta)));
		} else {
			model->setItem(i, 0, new QStandardItem(QIcon(":/icons/"+icons_theme+"/deselect_all.png"), ""));
			model->setItem(i, 1, new QStandardItem(meta.title()));
		}
	}
}

LibraryForm::LibraryForm(Library *lib, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LibraryForm)
{
	_lib = lib;
	Config config;
	_icons_theme = config.getValue("ui/iconstheme").toString();
	_model = new QStandardItemModel(this);
	_model->setColumnCount(2);
	_state = STATE_NONE;
	_tools_widget = new ToolsWidget(this);
	_search_in_library_button = new QPushButton(QIcon(":/icons/"+_icons_theme+"/search.png"), "", _tools_widget);
	_search_in_library_button->setFlat(true);
	_search_in_library_button->setCheckable(true);
	_tools_widget->layout()->addItem(new QSpacerItem(20, 20));
	_tools_widget->layout()->addWidget(_search_in_library_button);
	ui->setupUi(this);
	ui->listView->setModel(_model);
	ui->listView->setColumnWidth(0, 70);
	ui->toolsLayout->addWidget(_tools_widget);
	_tools_widget->hideFSButton();
	_tools_widget->hide();
	connect(ui->viewButton, SIGNAL(clicked()), this, SLOT(_view_button()));
	connect(ui->playlistsButton, SIGNAL(clicked()), this, SLOT(_playlists_button()));
	connect(ui->dynamicButton, SIGNAL(clicked()), this, SLOT(_dynamic_button()));
	connect(ui->listView, SIGNAL(clicked(QModelIndex)), this, SLOT(_process_list_click(QModelIndex)));
	connect(ui->listView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(_process_dblclick(QModelIndex)));
	connect(ui->listView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
		this, SLOT(_process_selection(QItemSelection,QItemSelection)));
	connect(ui->addButton, SIGNAL(clicked()), this, SLOT(_add_button()));
	connect(ui->selectAllButton, SIGNAL(clicked()), this, SLOT(_toggle_select_all_button()));
	connect(ui->backButton, SIGNAL(clicked()), this, SLOT(_back_button()));
	connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(_delete_button()));
	connect(ui->useButton, SIGNAL(clicked()), this, SLOT(_use_button()));
	connect(_tools_widget, SIGNAL(search(QString)), this, SLOT(search(QString)));
	connect(_tools_widget, SIGNAL(nextSearch()), this, SLOT(nextItem()));
	connect(_tools_widget, SIGNAL(prevSearch()), this, SLOT(prevItem()));
	connect(ui->moreButton, SIGNAL(clicked()), this, SLOT(_more_button()));
	connect(_search_in_library_button, SIGNAL(toggled(bool)), this, SLOT(_search_button(bool)));
	connect(ui->playerButton, SIGNAL(clicked()), this, SLOT(hide()));
	_view_button();
	_top_gradient = ui->topWidget->styleSheet();
	_bottom_gradient = ui->bottomWidget->styleSheet();
	_is_dynamic = false;
	_is_favorites = false;
	setAttribute(Qt::WA_Maemo5StackedWindow);
	setWindowFlags(Qt::Window | windowFlags());
	ui->addButton->setEnabled(false);
	ui->addButton->setIcon(QIcon());
	ui->deleteButton->setEnabled(false);
	ui->deleteButton->setIcon(QIcon());
}

LibraryForm::~LibraryForm()
{
	delete ui;
}

void LibraryForm::_view_button() {
	QList<QString> artitst = _lib->getArtists();
	__fill_model(_model, artitst, _icons_theme);
	ui->listView->setColumnWidth(0, 70);
	_state = STATE_ARTIST;
	ui->backButton->setEnabled(false);
	ui->backButton->setIcon(QIcon());
	ui->listLabel->setText(tr("Artists"));
	ui->useButton->setEnabled(false);
	ui->useButton->setIcon(QIcon());
	ui->addButton->setEnabled(false);
	ui->addButton->setIcon(QIcon());
	ui->deleteButton->setEnabled(false);
	ui->deleteButton->setIcon(QIcon());
}

void LibraryForm::_dynamic_button() {
	ui->useButton->setEnabled(false);
	ui->useButton->setIcon(QIcon());
	ui->backButton->setEnabled(false);
	ui->backButton->setIcon(QIcon());
	ui->deleteButton->setEnabled(false);
	ui->deleteButton->setIcon(QIcon());
	_model->clear();
	_model->setRowCount(4);
	_model->setItem(0, 0, new QStandardItem(QIcon(":/icons/"+_icons_theme+"/deselect_all.png"), ""));
	_model->setItem(0, 1, new QStandardItem("Favorites"));
	_model->setItem(1, 0, new QStandardItem(QIcon(":/icons/"+_icons_theme+"/deselect_all.png"), ""));
	_model->setItem(1, 1, new QStandardItem("Most played"));
	_model->setItem(2, 0, new QStandardItem(QIcon(":/icons/"+_icons_theme+"/deselect_all.png"), ""));
	_model->setItem(2, 1, new QStandardItem("Never played"));
	_model->setItem(3, 0, new QStandardItem(QIcon(":/icons/"+_icons_theme+"/deselect_all.png"), ""));
	_model->setItem(3, 1, new QStandardItem("Recently added"));
	ui->listView->setColumnWidth(0, 70);
	ui->listLabel->setText(tr("Dynamic playlists"));
	_state = STATE_DYNAMIC;
	ui->addButton->setEnabled(false);
	ui->addButton->setIcon(QIcon());
	ui->deleteButton->setEnabled(false);
	ui->deleteButton->setIcon(QIcon());
}

void LibraryForm::_process_list_click(QModelIndex index) {
	if (index.column() == 0) {
		return;
	}
	if (_state == STATE_NONE) return;
	QString data = index.data().toString();
	QRegExp regexp("\\[\\d+\\]\\ (.*)");
	switch (_state) {
	case STATE_ARTIST:
		_artist_row_for_scroll_to = index.row();
		__fill_model_album(_model, _lib->getAlbumsForArtist(data), _icons_theme);
		ui->listView->setColumnWidth(0, 70);
		ui->listView->scrollToTop();
		_current_artist = data;
		_state = STATE_ALBUM;
		ui->backButton->setEnabled(true);
		ui->backButton->setIcon(QIcon(":/icons/"+_icons_theme+"/back.png"));
		ui->listLabel->setText(QString(tr("Albums by \"%1\"")).arg(_current_artist));
		break;
	case STATE_ALBUM:
		_album_row_for_scroll_to = index.row();
		if (regexp.indexIn(data) != -1) {
			_current_album = regexp.cap(1).trimmed();
			_current_tracks = _lib->getTracksForAlbum(_current_album, _current_artist);
			__fill_model_tracks(_model, _current_tracks, _icons_theme);
			ui->listView->setColumnWidth(0, 70);
			ui->listView->scrollToTop();
			_state = STATE_TRACK;
			ui->backButton->setEnabled(true);
			ui->backButton->setIcon(QIcon(":/icons/"+_icons_theme+"/back.png"));
			ui->listLabel->setText(QString(tr("Tracks from \"%1\" by \"%2\"")).arg(_current_album).arg(_current_artist));
		} else {
			if (index.row() == 0) { // all tracks
				_current_tracks = _lib->getAllTracksForArtist(_current_artist);
				__fill_model_tracks(_model, _current_tracks, _icons_theme);
				ui->listView->setColumnWidth(0, 70);
				ui->listView->scrollToTop();
				_state = STATE_TRACK;
				ui->backButton->setEnabled(true);
				ui->backButton->setIcon(QIcon(":/icons/"+_icons_theme+"/back.png"));
				ui->listLabel->setText(QString(tr("All tracks by \"%1\"")).arg(_current_artist));
			}
		}
		break;
	case STATE_PLAYLIST:
		{
			_playlist_row_for_scroll_to = index.row();
			_current_playlist = _lib->getPlaylist(data);
			_current_tracks = _current_playlist.tracks();
			__fill_model_tracks(_model, _current_tracks, _icons_theme);
			ui->listView->setColumnWidth(0, 70);
			ui->listView->scrollToTop();
			_state = STATE_PLAYLIST_TRACK;
			_is_dynamic = false;
			ui->backButton->setEnabled(true);
			ui->backButton->setIcon(QIcon(":/icons/"+_icons_theme+"/back.png"));
			ui->useButton->setEnabled(true);
			ui->useButton->setIcon(QIcon(":/icons/"+_icons_theme+"/use.png"));
			ui->listLabel->setText(QString(tr("Tracks in playlist \"%1\"")).arg(data));
		}
		break;
	case STATE_DYNAMIC:
		{
			switch(index.row()) {
			case 0: //favorites
				_current_playlist = _lib->getFavorites();
				_is_favorites = true;
				break;
			case 1: //most played
				_current_playlist = _lib->getMostPlayed();
				_is_favorites = false;
				break;
			case 2: //never played
				_current_playlist = _lib->getNeverPlayed();
				_is_favorites = false;
				break;
			case 3: //recently added
				_current_playlist = _lib->getRecentlyAdded();
				_is_favorites = false;
				break;
			default:
				return;
			}
			_current_tracks = _current_playlist.tracks();
			__fill_model_tracks(_model, _current_tracks, _icons_theme);
			ui->listView->setColumnWidth(0, 70);
			ui->listView->scrollToTop();
			_state = STATE_PLAYLIST_TRACK;
			_is_dynamic = true;
			ui->backButton->setEnabled(true);
			ui->backButton->setIcon(QIcon(":/icons/"+_icons_theme+"/back.png"));
			ui->useButton->setEnabled(true);
			ui->useButton->setIcon(QIcon(":/icons/"+_icons_theme+"/use.png"));
			ui->listLabel->setText(_current_playlist.name());
		}
	default:
		return;
	}
	ui->addButton->setEnabled(false);
	ui->addButton->setIcon(QIcon());
	ui->useButton->setEnabled(false);
	ui->useButton->setIcon(QIcon());
	ui->deleteButton->setEnabled(false);
	ui->deleteButton->setIcon(QIcon());
	QTimer::singleShot(100, ui->listView, SLOT(clearSelection())); // workaround
}

void LibraryForm::_add_button() {
	if (_state == STATE_NONE) return;
	QModelIndexList selected = ui->listView->selectionModel()->selectedIndexes();
	ui->listView->selectionModel()->clearSelection();
	Playlist cur = _lib->getCurrentPlaylist();
	QRegExp regexp("\\[\\d+\\]\\ (.*)");
	switch (_state) {
	case STATE_ARTIST:
		foreach (QModelIndex id, selected) {
			_add_artist(&cur, id.data().toString());
		}
		_lib->saveCurrentPlaylist(cur);
		emit refreshPlayer();
		break;
	case STATE_ALBUM:
		foreach (QModelIndex id, selected) {
			if (regexp.indexIn(id.data().toString()) != -1) {
				_add_album(&cur, _current_artist, regexp.cap(1).trimmed());
			}
		}
		_lib->saveCurrentPlaylist(cur);
		emit refreshPlayer();
		break;
	case STATE_TRACK:
		foreach (QModelIndex id, selected) {
			_add_track(&cur, _current_tracks.at(id.row()));
		}
		_lib->saveCurrentPlaylist(cur);
		emit refreshPlayer();
		break;
	case STATE_PLAYLIST:
		foreach (QModelIndex id, selected) {
			_add_playlist(&cur, id.data().toString());
		}
		_lib->saveCurrentPlaylist(cur);
		emit refreshPlayer();
		break;
	case STATE_DYNAMIC:

		break;
	case STATE_PLAYLIST_TRACK:
		foreach (QModelIndex id, selected) {
			_add_track(&cur, _current_tracks.at(id.row()));
		}
		_lib->saveCurrentPlaylist(cur);
		emit refreshPlayer();
		break;
	case STATE_SEARCH:
		foreach (QModelIndex id, selected) {
			_add_track(&cur, _current_tracks.at(id.row()));
		}
		_lib->saveCurrentPlaylist(cur);
		emit refreshPlayer();
		break;
	default:
		return;
	}
}


void LibraryForm::_add_artist(Playlist *cur, QString artist) {
	QMap<QString, int> albums = _lib->getAlbumsForArtist(artist);
	foreach(QString album, albums.keys()) {
		_add_album(cur, artist, album);
	}
}

void LibraryForm::_add_album(Playlist *cur, QString artist, QString album) {
	QList<Track> tracks = _lib->getTracksForAlbum(album, artist);
	foreach(Track track, tracks) {
		_add_track(cur, track);
	}
}

void LibraryForm::_add_track(Playlist *cur, Track track) {
	cur->addTrack(track);
}

void LibraryForm::_add_playlist(Playlist *cur, QString name) {
	Playlist playlist = _lib->getPlaylist(name);
	QList<Track> tracks = playlist.tracks();
	foreach (Track track, tracks) {
		_add_track(cur, track);
	}
}

void LibraryForm::_back_button() {
	switch (_state) {
	case STATE_ALBUM:
		_view_button();
		if (_model->rowCount() != 0) {
			ui->listView->scrollTo(_model->index(_artist_row_for_scroll_to, 1), QAbstractItemView::PositionAtCenter);
		}
		break;
	case STATE_TRACK:
		__fill_model_album(_model, _lib->getAlbumsForArtist(_current_artist), _icons_theme);
		ui->listView->setColumnWidth(0, 70);
		if (_model->rowCount() != 0) {
			ui->listView->scrollTo(_model->index(_album_row_for_scroll_to, 1), QAbstractItemView::PositionAtCenter);
		}
		_state = STATE_ALBUM;
		ui->listLabel->setText(QString(tr("Albums by \"%1\"")).arg(_current_artist));
		break;
	case STATE_PLAYLIST_TRACK:
		if (_is_dynamic) {
			_dynamic_button();
			_is_dynamic = false;
		} else {
			_playlists_button();
		}
		if (_model->rowCount() != 0 && !_is_dynamic) {
			ui->listView->scrollTo(_model->index(_playlist_row_for_scroll_to, 1), QAbstractItemView::PositionAtCenter);
		}
	default:
		return;
	}
}

void LibraryForm::_playlists_button() {
	QList<QString> playlists = _lib->getPlaylistsNames();
	__fill_model(_model, playlists, _icons_theme);
	ui->listView->setColumnWidth(0, 70);
	_state = STATE_PLAYLIST;
	ui->backButton->setEnabled(false);
	ui->backButton->setIcon(QIcon());
	ui->listLabel->setText(tr("Playlists"));
	ui->useButton->setEnabled(false);
	ui->useButton->setIcon(QIcon());
	ui->addButton->setEnabled(false);
	ui->addButton->setIcon(QIcon());
	ui->deleteButton->setEnabled(false);
	ui->deleteButton->setIcon(QIcon());
}

void LibraryForm::_delete_button() {
	if (_state == STATE_PLAYLIST_TRACK) {
		QModelIndexList selected = ui->listView->selectionModel()->selectedIndexes();
		ui->listView->selectionModel()->clearSelection();
		QQueue<int> to_delete;
		foreach (QModelIndex id, selected) {
			if (!to_delete.contains(id.row()))
				to_delete.append(id.row());
		}
		qSort(to_delete);
		int count = to_delete.count();
		if (_is_dynamic && _is_favorites) {
			_current_tracks = _lib->getFavorites().tracks();
			for (int i = count-1; i >= 0; i--) {
				_lib->removeFromFavorites(_current_tracks.at(to_delete.at(i)));
			}
			_current_playlist = _lib->getFavorites();
			_current_tracks = _current_playlist.tracks();
		} else {
			for (int i = count-1; i >= 0; i--) {
				_current_playlist.removeTrackAt(to_delete.at(i));
			}
			_current_tracks = _current_playlist.tracks();
			_lib->savePlaylist(_current_playlist);
			emit refreshPlayer();
		}
		__fill_model_tracks(_model, _current_tracks, _icons_theme);
		ui->listView->setColumnWidth(0, 70);
	} else if (_state == STATE_PLAYLIST) {
		QModelIndexList selected = ui->listView->selectionModel()->selectedIndexes();
		QQueue<int> to_delete;
		foreach (QModelIndex id, selected) {
			if (!to_delete.contains(id.row()))
				to_delete.append(id.row());
		}
		qSort(to_delete);
		int count = to_delete.count();
		for (int i = count-1; i >= 0; i--) {
			QString name = _model->item(to_delete.at(i), 1)->text();
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
	emit refreshPlayer();
}

void LibraryForm::_use_button() {
	if (_state == STATE_PLAYLIST_TRACK) {
		_lib->saveCurrentPlaylist(_current_playlist);
		emit refreshPlayer();
		_current_playlist = _lib->getCurrentPlaylist();
		return;
	}

	if (_state == STATE_NONE) return;
	QModelIndexList selected = ui->listView->selectionModel()->selectedIndexes();
	ui->listView->selectionModel()->clearSelection();
	Playlist cur;
	QRegExp regexp("\\[\\d+\\]\\ (.*)");
	switch (_state) {
	case STATE_ARTIST:
		foreach (QModelIndex id, selected) {
			_add_artist(&cur, id.data().toString());
		}
		_lib->saveCurrentPlaylist(cur);
		emit refreshPlayer();
		break;
	case STATE_ALBUM:
		foreach (QModelIndex id, selected) {
			if (regexp.indexIn(id.data().toString()) != -1) {
				_add_album(&cur, _current_artist, regexp.cap(1).trimmed());
			}
		}
		_lib->saveCurrentPlaylist(cur);
		emit refreshPlayer();
		break;
	case STATE_TRACK:
		foreach (QModelIndex id, selected) {
			_add_track(&cur, _current_tracks.at(id.row()));
		}
		_lib->saveCurrentPlaylist(cur);
		emit refreshPlayer();
		break;
	case STATE_PLAYLIST:
		foreach (QModelIndex id, selected) {
			_add_playlist(&cur, id.data().toString());
		}
		_lib->saveCurrentPlaylist(cur);
		emit refreshPlayer();
		break;
	case STATE_DYNAMIC:

		break;
	case STATE_SEARCH:
		foreach (QModelIndex id, selected) {
			_add_track(&cur, _current_tracks.at(id.row()));
		}
		_lib->saveCurrentPlaylist(cur);
		emit refreshPlayer();
		break;
	default:
		return;
	}
}

void LibraryForm::search(QString pattern) {
	_search_pattern = pattern;
	_search_current_id = -1;
	nextItem();
}

void LibraryForm::nextItem() {
	QString data = _model->index(_search_current_id, 0).data().toString();
	for (int i = _search_current_id+1; i < _model->rowCount(); i++) {
		data = _model->index(i, 1).data().toString();
		if (data.contains(_search_pattern, Qt::CaseInsensitive)) {
			_search_current_id = i;
			break;
		}
	}
	QModelIndex id = _model->index(_search_current_id, 1);
	ui->listView->selectionModel()->clearSelection();
	ui->listView->selectRow(id.row());
	ui->listView->scrollTo(id, QAbstractItemView::PositionAtCenter);
}

void LibraryForm::prevItem() {
	QString data = _model->index(_search_current_id, 0).data().toString();
	for (int i = _search_current_id-1; i >= 0; i--) {
		data = _model->index(i, 1).data().toString();
		if (data.contains(_search_pattern, Qt::CaseInsensitive)) {
			_search_current_id = i;
			break;
		}
	}
	QModelIndex id = _model->index(_search_current_id, 1);
	ui->listView->selectionModel()->clearSelection();
	ui->listView->selectRow(id.row());
	ui->listView->scrollTo(id, QAbstractItemView::PositionAtCenter);
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
		__fill_model_album(_model, _lib->getAlbumsForArtist(_current_artist), _icons_theme);
		ui->listView->setColumnWidth(0, 70);
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
		__fill_model_tracks(_model, _current_tracks, _icons_theme);
		ui->listView->setColumnWidth(0, 70);
		break;
	case STATE_TRACK:
		_current_tracks = _lib->getTracksForAlbum(_current_album, _current_artist);
		__fill_model_tracks(_model, _current_tracks, _icons_theme);
		break;
	default:
		return;
	}
}

void LibraryForm::_toggle_select_all_button() {
	if (ui->listView->selectionModel()->selectedIndexes().count() == _model->rowCount() * _model->columnCount()) {
		ui->listView->selectionModel()->clearSelection();
		ui->selectAllButton->setIcon(QIcon(":/icons/"+_icons_theme+"/select_all.png"));
	} else {
		disconnect(ui->listView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
			   this, SLOT(_process_selection(QItemSelection,QItemSelection)));
		ui->listView->selectAll();
		int cnt = _model->rowCount();
		for (int i = 0; i < cnt; i++) {
			_model->item(i)->setIcon(QIcon(":/icons/"+_icons_theme+"/select_all.png"));
		}
		connect(ui->listView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
			   this, SLOT(_process_selection(QItemSelection,QItemSelection)));
		ui->selectAllButton->setIcon(QIcon(":/icons/"+_icons_theme+"/deselect_all.png"));
		if (_state != STATE_DYNAMIC) {
			ui->addButton->setIcon(QIcon(":/icons/"+_icons_theme+"/add.png"));
			ui->addButton->setEnabled(true);
			ui->useButton->setEnabled(true);
			ui->useButton->setIcon(QIcon(":/icons/"+_icons_theme+"/use.png"));
		}

		if (_state == STATE_PLAYLIST || (_state == STATE_PLAYLIST_TRACK && !_is_dynamic)
			|| (_state == STATE_PLAYLIST_TRACK && _is_favorites)) {
			ui->deleteButton->setEnabled(true);
			ui->deleteButton->setIcon(QIcon(":/icons/"+_icons_theme+"/delete.png"));
		}
	}
}

void LibraryForm::landscapeMode() {
	landscape = true;
	ui->topWidget->hide();
	ui->bottomWidget->hide();

	ui->lverticalLayout->removeItem(ui->lverticalSpacer_0);
	ui->lverticalLayout->removeItem(ui->lverticalSpacer_1);
	ui->lverticalLayout->removeItem(ui->lverticalSpacer_2);
	ui->lverticalLayout->removeItem(ui->lverticalSpacer_3);
	ui->lverticalLayout->addWidget(ui->backButton);
	ui->lverticalLayout->addItem(ui->lverticalSpacer_0);
	ui->lverticalLayout->addWidget(ui->addButton);
	ui->lverticalLayout->addItem(ui->lverticalSpacer_1);
	ui->lverticalLayout->addWidget(ui->deleteButton);
	ui->lverticalLayout->addItem(ui->lverticalSpacer_2);
	ui->lverticalLayout->addWidget(ui->useButton);
	ui->lverticalLayout->addItem(ui->lverticalSpacer_3);
	ui->lverticalLayout->addWidget(ui->playerButton);

	ui->rverticalLayout->removeItem(ui->rverticalSpacer_0);
	ui->rverticalLayout->removeItem(ui->rverticalSpacer_1);
	ui->rverticalLayout->removeItem(ui->rverticalSpacer_2);
	ui->rverticalLayout->removeItem(ui->rverticalSpacer_3);
	ui->rverticalLayout->addWidget(ui->moreButton);
	ui->rverticalLayout->addItem(ui->rverticalSpacer_0);
	ui->rverticalLayout->addWidget(ui->selectAllButton);
	ui->rverticalLayout->addItem(ui->rverticalSpacer_1);
	ui->rverticalLayout->addWidget(ui->viewButton);
	ui->rverticalLayout->addItem(ui->rverticalSpacer_2);
	ui->rverticalLayout->addWidget(ui->dynamicButton);
	ui->rverticalLayout->addItem(ui->rverticalSpacer_3);
	ui->rverticalLayout->addWidget(ui->playlistsButton);

	ui->lverticalWidget->show();
	ui->rverticalWidget->show();

	if (_tools_widget->isVisible()) {
		ui->moreButton->setIcon(QIcon(":/icons/"+_icons_theme+"/more_l.png"));
	} else {
		ui->moreButton->setIcon(QIcon(":/icons/"+_icons_theme+"/unmore_l.png"));
	}
}

void LibraryForm::portraitMode() {
	landscape = false;

	ui->lverticalWidget->hide();
	ui->rverticalWidget->hide();

	ui->lverticalLayout->removeItem(ui->lverticalSpacer_0);
	ui->lverticalLayout->removeItem(ui->lverticalSpacer_1);
	ui->lverticalLayout->removeItem(ui->lverticalSpacer_2);
	ui->lverticalLayout->removeItem(ui->lverticalSpacer_3);

	ui->rverticalLayout->removeItem(ui->rverticalSpacer_0);
	ui->rverticalLayout->removeItem(ui->rverticalSpacer_1);
	ui->rverticalLayout->removeItem(ui->rverticalSpacer_2);
	ui->rverticalLayout->removeItem(ui->rverticalSpacer_3);

	ui->topWidget->layout()->removeItem(ui->thorizontalSpacer_0);
	ui->topWidget->layout()->removeItem(ui->thorizontalSpacer_1);
	ui->topWidget->layout()->removeItem(ui->thorizontalSpacer_2);
	ui->topWidget->layout()->removeItem(ui->thorizontalSpacer_3);
	ui->topWidget->layout()->removeItem(ui->thorizontalSpacer_4);
	ui->topWidget->layout()->addWidget(ui->backButton);
	ui->topWidget->layout()->addItem(ui->thorizontalSpacer_0);
	ui->topWidget->layout()->addWidget(ui->deleteButton);
	ui->topWidget->layout()->addItem(ui->thorizontalSpacer_1);
	ui->topWidget->layout()->addWidget(ui->addButton);
	ui->topWidget->layout()->addItem(ui->thorizontalSpacer_2);
	ui->topWidget->layout()->addWidget(ui->useButton);
	ui->topWidget->layout()->addItem(ui->thorizontalSpacer_3);
	ui->topWidget->layout()->addWidget(ui->selectAllButton);
	ui->topWidget->layout()->addItem(ui->thorizontalSpacer_4);
	ui->topWidget->layout()->addWidget(ui->moreButton);

	ui->bottomWidget->layout()->removeItem(ui->bhorizontalSpacer_0);
	ui->bottomWidget->layout()->removeItem(ui->bhorizontalSpacer_1);
	ui->bottomWidget->layout()->removeItem(ui->bhorizontalSpacer_2);
	ui->bottomWidget->layout()->addWidget(ui->playerButton);
	ui->bottomWidget->layout()->addItem(ui->bhorizontalSpacer_0);
	ui->bottomWidget->layout()->addWidget(ui->viewButton);
	ui->bottomWidget->layout()->addItem(ui->bhorizontalSpacer_1);
	ui->bottomWidget->layout()->addWidget(ui->dynamicButton);
	ui->bottomWidget->layout()->addItem(ui->bhorizontalSpacer_2);
	ui->bottomWidget->layout()->addWidget(ui->playlistsButton);

	ui->topWidget->show();
	ui->bottomWidget->show();

	if (_tools_widget->isVisible()) {
		ui->moreButton->setIcon(QIcon(":/icons/"+_icons_theme+"/unmore.png"));
	} else {
		ui->moreButton->setIcon(QIcon(":/icons/"+_icons_theme+"/more.png"));
	}
}

void LibraryForm::_more_button() {
	if (_tools_widget->isVisible()) {
		ui->moreButton->setIcon(QIcon(landscape ? ":/icons/"+_icons_theme+"/unmore_l.png" : ":/icons/"+_icons_theme+"/more.png"));
		_tools_widget->hide();
		_tools_widget->reset();
		cancelSearch();
	} else {
		ui->moreButton->setIcon(QIcon(landscape ? ":/icons/"+_icons_theme+"/more_l.png" : ":/icons/"+_icons_theme+"/unmore.png"));
		_tools_widget->show();
		_tools_widget->setFocus();
	}
}


void LibraryForm::updateIcons() {
	Config config;
	_icons_theme = config.getValue("ui/iconstheme").toString();
	_tools_widget->updateIcons();
	if (!ui->addButton->icon().isNull()) {
		ui->addButton->setIcon(QIcon(":/icons/"+_icons_theme+"/add.png"));
	}
	if (!ui->deleteButton->icon().isNull()) {
		ui->deleteButton->setIcon(QIcon(":/icons/"+_icons_theme+"/delete.png"));
	}
	if (!ui->useButton->icon().isNull()) {
		ui->useButton->setIcon(QIcon(":/icons/"+_icons_theme+"/use.png"));
	}
	if (!ui->backButton->icon().isNull()) {
		ui->backButton->setIcon(QIcon(":/icons/"+_icons_theme+"/back.png"));
	}
	if (!ui->dynamicButton->icon().isNull()) {
		ui->dynamicButton->setIcon(QIcon(":/icons/"+_icons_theme+"/dynamic.png"));
	}
	if (_tools_widget->isVisible()) {
		ui->moreButton->setIcon(QIcon(landscape ? ":/icons/"+_icons_theme+"/more_l.png" : ":/icons/"+_icons_theme+"/unmore.png"));
	} else {
		ui->moreButton->setIcon(QIcon(landscape ? ":/icons/"+_icons_theme+"/unmore_l.png" : ":/icons/"+_icons_theme+"/more.png"));
	}
	if (!ui->playlistsButton->icon().isNull()) {
		ui->playlistsButton->setIcon(QIcon(":/icons/"+_icons_theme+"/playlists.png"));
	}
	ui->playerButton->setIcon(QIcon(":/icons/"+_icons_theme+"/player.png"));
	if (!ui->viewButton->icon().isNull()) {
		ui->viewButton->setIcon(QIcon(":/icons/"+_icons_theme+"/artists.png"));
	}
	if (ui->listView->selectionModel()->selectedRows().count() == _model->rowCount()) {
		ui->selectAllButton->setIcon(QIcon(":/icons/"+_icons_theme+"/deselect_all.png"));
	} else {
		ui->selectAllButton->setIcon(QIcon(":/icons/"+_icons_theme+"/select_all.png"));
	}
	_search_in_library_button->setIcon(QIcon(":/icons/"+_icons_theme+"/search.png"));
	refresh();
}

void LibraryForm::checkGradient() {
	Config config;
	if (config.getValue("ui/gradient").toString() == "yes") {
		ui->bottomWidget->setStyleSheet(_bottom_gradient);
		ui->topWidget->setStyleSheet(_top_gradient);
	} else {
		ui->topWidget->setStyleSheet("");
		ui->bottomWidget->setStyleSheet("");
	}
}

void LibraryForm::_process_selection(QItemSelection selected, QItemSelection deselected) {
	foreach (QModelIndex id, selected.indexes()) {
		if (id.column() == 0) {
			_model->item(id.row())->setIcon(QIcon(":/icons/"+_icons_theme+"/select_all.png"));
		}
		ui->listView->selectionModel()->select(id, QItemSelectionModel::Select);
	}
	foreach (QModelIndex id, deselected.indexes()) {
		if (id.column() == 0) {
			_model->item(id.row())->setIcon(QIcon(":/icons/"+_icons_theme+"/deselect_all.png"));
		}
		ui->listView->selectionModel()->select(id, QItemSelectionModel::Deselect);
	}
	if (ui->listView->selectionModel()->selectedRows().count() > 0 ) {
		if (_state == STATE_DYNAMIC) {
			return;
		}
		ui->addButton->setEnabled(true);
		ui->addButton->setIcon(QIcon(":/icons/"+_icons_theme+"/add.png"));
		ui->useButton->setEnabled(true);
		ui->useButton->setIcon(QIcon(":/icons/"+_icons_theme+"/use.png"));
		if (_state == STATE_PLAYLIST || (_state == STATE_PLAYLIST_TRACK && !_is_dynamic)
			|| (_state == STATE_PLAYLIST_TRACK && _is_favorites)) {
			ui->deleteButton->setEnabled(true);
			ui->deleteButton->setIcon(QIcon(":/icons/"+_icons_theme+"/delete.png"));
		}
	} else {
		ui->addButton->setEnabled(false);
		ui->addButton->setIcon(QIcon());
		if (_state != STATE_PLAYLIST_TRACK) {
			ui->useButton->setEnabled(false);
			ui->useButton->setIcon(QIcon());
		}
		ui->deleteButton->setEnabled(false);
		ui->deleteButton->setIcon(QIcon());
	}
}

void LibraryForm::_process_dblclick(QModelIndex id) {
	if (id.column() == 0)
		return;
	if (_state == STATE_TRACK || _state == STATE_PLAYLIST_TRACK || _state == STATE_SEARCH) {
		Playlist cur = _lib->getCurrentPlaylist();
		Track track = _current_tracks.at(id.row());
		cur.addTrack(track);
		_lib->saveCurrentPlaylist(cur);
		emit refreshPlayer();
		emit addAndPlay(track);
		ui->listView->clearSelection();
	}
}

void LibraryForm::_search_button(bool state) {
	ui->moreButton->setEnabled(!state);
	_tools_widget->toggleArrows(!state);
	ui->backButton->setEnabled(!state);
	ui->backButton->setIcon(state ? QIcon() : QIcon(":/icons/"+_icons_theme+"/back.png"));
	_tools_widget->setFocus();
	_tools_widget->reset();
	if (state) {
		ui->listLabel->setText(tr("Search in library"));
		ui->deleteButton->setIcon(QIcon());
		ui->deleteButton->setEnabled(false);
		ui->useButton->setIcon(QIcon());
		ui->useButton->setEnabled(false);
		ui->viewButton->setIcon(QIcon());
		ui->viewButton->setEnabled(false);
		ui->dynamicButton->setIcon(QIcon());
		ui->dynamicButton->setEnabled(false);
		ui->playlistsButton->setIcon(QIcon());
		ui->playlistsButton->setEnabled(false);
		disconnect(_tools_widget, SIGNAL(search(QString)), this, SLOT(search(QString)));
		connect(_tools_widget, SIGNAL(search(QString)), this, SLOT(_search_in_library(QString)));
		_model->clear();
		_state = STATE_SEARCH;
	} else {
		ui->viewButton->setIcon(QIcon(":/icons/"+_icons_theme+"/artists.png"));
		ui->viewButton->setEnabled(true);
		ui->dynamicButton->setIcon(QIcon(":/icons/"+_icons_theme+"/dynamic.png"));
		ui->dynamicButton->setEnabled(true);
		ui->playlistsButton->setIcon(QIcon(":/icons/"+_icons_theme+"/playlists.png"));
		ui->playlistsButton->setEnabled(true);
		_view_button();
		connect(_tools_widget, SIGNAL(search(QString)), this, SLOT(search(QString)));
		disconnect(_tools_widget, SIGNAL(search(QString)), this, SLOT(_search_in_library(QString)));
	}
}

void LibraryForm::_search_in_library(QString pattern) {
	pattern = pattern.trimmed();
	if (pattern.isEmpty()) {
		_model->clear();
		return;
	}
	_current_tracks = _lib->searchTracks(pattern);
	__fill_model_tracks(_model, _current_tracks, _icons_theme);
	ui->listView->setColumnWidth(0, 70);
	ui->listView->scrollToTop();
}

void LibraryForm::updateTranslations() {
	ui->retranslateUi(this);
}
