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

#include "playlistsortform.h"
#include "ui_playlistsortform.h"
#include <QTime>
#include "track.h"
#include <QDebug>
#include "config.h"

using namespace SomePlayer::DataObjects;
using namespace SomePlayer::Storage;

inline void __fill_model(QStandardItemModel *_model, Playlist playlist) {
	_model->clear();
	QList<Track> tracks = playlist.tracks();
	int count = tracks.count();
	_model->setRowCount(count);
	_model->setColumnCount(2);
	QTime time;
	for (int i = 0; i < count; i++) {
		TrackMetadata meta = tracks.at(i).metadata();
		time.setHMS(0, meta.length()/60, meta.length() % 60);
		QString t = meta.title()+"#_#"+meta.artist()+"#_#"+meta.album()+"#_#"+time.toString("mm:ss");
		_model->setItem(i, 1, new QStandardItem(t));
		_model->setItem(i, 0, new QStandardItem("##deselect_all.png"));
	}
}

PlaylistSortForm::PlaylistSortForm(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::PlaylistSortForm)
{
	ui->setupUi(this);
	setAttribute(Qt::WA_Maemo5StackedWindow);
	setWindowFlags(Qt::Window | windowFlags());
	_track_renderer = new TrackRenderer(this);
	_track_renderer->setActiveRow(-1);
	_track_renderer->setSearchRow(-1);
	ui->playlistView->setItemDelegateForColumn(0, _track_renderer);
	ui->playlistView->setItemDelegateForColumn(1, _track_renderer);
	_model = new QStandardItemModel(this);
	ui->playlistView->setModel(_model);
	_top_gradient = ui->topWidget->styleSheet();
	_bottom_gradient = ui->bottomWidget->styleSheet();

	connect(ui->doneButton, SIGNAL(clicked()), this, SLOT(done()));
	connect(ui->playlistView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
		this, SLOT(_process_selection(QItemSelection,QItemSelection)));
	connect(ui->moveDownButton, SIGNAL(clicked()), this, SLOT(moveDown()));
	connect(ui->moveTopButton, SIGNAL(clicked()), this, SLOT(moveUp()));
	connect(ui->sortAlphAscButton, SIGNAL(clicked()), this, SLOT(sortAlphAsc()));
	connect(ui->sortAlphDescButton, SIGNAL(clicked()), this, SLOT(sortAlphDesc()));
	connect(ui->shuffleButton, SIGNAL(clicked()), this, SLOT(shuffle()));
	connect(ui->artistsButton, SIGNAL(clicked()), this, SLOT(sortArtist()));
}

PlaylistSortForm::~PlaylistSortForm()
{
	delete ui;
}

void PlaylistSortForm::done() {
	emit playlistChanged();
	hide();
}

void PlaylistSortForm::setPlaylist(Playlist playlist) {
	_playlist = playlist;
	updateView();
}

Playlist PlaylistSortForm::getPlaylist() {
	return _playlist;
}

void PlaylistSortForm::updateView() {
	__fill_model(_model, _playlist);
	ui->playlistView->setColumnWidth(0, 50);
}

void PlaylistSortForm::_process_selection(QItemSelection selected, QItemSelection deselected) {
	foreach (QModelIndex id, selected.indexes()) {
		if (id.column() == 0) {
			_model->item(id.row(), 0)->setText("##select_all.png");
		}
		ui->playlistView->selectionModel()->select(id, QItemSelectionModel::Select);
	}
	foreach (QModelIndex id, deselected.indexes()) {
		if (id.column() == 0) {
			_model->item(id.row(), 0)->setText("##deselect_all.png");
		}
		ui->playlistView->selectionModel()->select(id, QItemSelectionModel::Deselect);
	}
}

void PlaylistSortForm::moveDown() {
	QModelIndexList idx = ui->playlistView->selectionModel()->selectedIndexes();
	if (idx.isEmpty()) {
		return;
	}
	QList<int> rows;
	foreach (QModelIndex id, idx) {
		if (id.column() == 1) {
			rows << id.row();
		}
	}
	_move(rows, 1);
}

void PlaylistSortForm::moveUp() {
	QModelIndexList idx = ui->playlistView->selectionModel()->selectedIndexes();
	if (idx.isEmpty()) {
		return;
	}
	QList<int> rows;
	foreach (QModelIndex id, idx) {
		if (id.column() == 1) {
			rows << id.row();
		}
	}
	_move(rows, -1);
}

void PlaylistSortForm::sortAlphAsc() {
	QList<Track> tracks = _playlist.tracks();
	qSort(tracks);
	_playlist.setTracks(tracks);
	updateView();
}

void PlaylistSortForm::sortAlphDesc() {
	QList<Track> tracks = _playlist.tracks();
	qSort(tracks);
	for(int i = 0; i < (tracks.size()/2); i++) {
		tracks.swap(i,tracks.size()-(1+i));
	}
	_playlist.setTracks(tracks);
	updateView();
}

void PlaylistSortForm::sortArtist() {
	QList<Track> tracks = _playlist.tracks();
	QList<Track> ntracks;
	// first step: artists
	QMap<QString, QList<Track> > artists_tracks;
	foreach (Track t, tracks) {
		artists_tracks[t.metadata().artist()].append(t);;
	}
	// second step: albums;
	QMap<QString, QList<Track> > album_tracks;
	QList<QString> artists = artists_tracks.keys();
	qSort(artists);
	foreach (QString artist, artists) {
		album_tracks.clear();
		foreach (Track t, artists_tracks[artist]) {
			album_tracks[t.metadata().album()].append(t);
		}
		QList<QString> albums = album_tracks.keys();
		qSort(albums);
		foreach (QString album, albums) {
			qWarning() << album;
			foreach (Track t, album_tracks[album]) {
				ntracks << t;
			}
		}
	}
	_playlist.setTracks(ntracks);
	updateView();
}

void PlaylistSortForm::shuffle() {
	qsrand(QTime::currentTime().msec());
	QList<Track> tracks = _playlist.tracks();
	QList<Track> ntracks;
	int size = tracks.size();
	int pos = 0;
	for (int i = 0; i < size; i++) {
		pos = qrand() % tracks.count();
		ntracks << tracks.at(pos);
		tracks.removeAt(pos);
	}
	_playlist.setTracks(ntracks);
	updateView();
}

void PlaylistSortForm::_move(QList<int> rows, int diff) {
	qSort(rows);
	QList<Track> tracks = _playlist.tracks();
	if ((diff > 0 && (rows.last() == tracks.count()-diff)) ||
	    (diff < 0 && (rows.first() == -diff - 1))) {
		return;
	}
	if (diff > 0) {
		for (int i = rows.count()-1; i>=0; i--) {
			tracks.swap(rows[i], rows[i]+diff);
			rows[i] += diff;
		}
	} else {
		for (int i = 0; i < rows.count(); i++) {
			tracks.swap(rows[i], rows[i]+diff);
			rows[i] += diff;
		}
	}
	_playlist.setTracks(tracks);
	updateView();
	foreach (int i, rows) {
		ui->playlistView->selectRow(i);
	}
	ui->playlistView->scrollTo(_model->index(rows[rows.count()/2], 0), QAbstractItemView::PositionAtCenter);
}

void PlaylistSortForm::landscapeMode() {
	ui->topWidget->hide();
	ui->bottomWidget->hide();
	ui->bhorWidget->hide();

	ui->topWidget->layout()->removeItem(ui->thorizontalSpacer_0);
	ui->topWidget->layout()->removeItem(ui->thorizontalSpacer_1);
	ui->topWidget->layout()->removeWidget(ui->sortAlphAscButton);
	ui->topWidget->layout()->removeWidget(ui->sortAlphDescButton);
	ui->topWidget->layout()->removeWidget(ui->artistsButton);

	ui->bottomWidget->layout()->removeItem(ui->bhorizontalSpacer_0);
	ui->bottomWidget->layout()->removeItem(ui->bhorizontalSpacer_1);
	ui->bottomWidget->layout()->removeItem(ui->bhorizontalSpacer_2);
	ui->bottomWidget->layout()->removeWidget(ui->doneButton);
	ui->bottomWidget->layout()->removeWidget(ui->moveDownButton);
	ui->bottomWidget->layout()->removeWidget(ui->moveTopButton);
	ui->bottomWidget->layout()->removeWidget(ui->shuffleButton);

	ui->bhorWidget->layout()->removeWidget(ui->doneButton);
	ui->bhorWidget->layout()->removeItem(ui->chorizontalSpacer_0);
	ui->bhorWidget->layout()->removeWidget(ui->moveTopButton);
	ui->bhorWidget->layout()->removeItem(ui->chorizontalSpacer_1);
	ui->bhorWidget->layout()->removeWidget(ui->sortAlphAscButton);
	ui->bhorWidget->layout()->removeItem(ui->chorizontalSpacer_2);
	ui->bhorWidget->layout()->removeWidget(ui->artistsButton);
	ui->bhorWidget->layout()->removeItem(ui->chorizontalSpacer_3);
	ui->bhorWidget->layout()->removeWidget(ui->sortAlphDescButton);
	ui->bhorWidget->layout()->removeItem(ui->chorizontalSpacer_4);
	ui->bhorWidget->layout()->removeWidget(ui->moveDownButton);
	ui->bhorWidget->layout()->removeItem(ui->chorizontalSpacer_5);
	ui->bhorWidget->layout()->removeWidget(ui->shuffleButton);

	ui->bhorWidget->layout()->addWidget(ui->doneButton);
	ui->bhorWidget->layout()->addItem(ui->chorizontalSpacer_0);
	ui->bhorWidget->layout()->addWidget(ui->sortAlphAscButton);
	ui->bhorWidget->layout()->addItem(ui->chorizontalSpacer_1);
	ui->bhorWidget->layout()->addWidget(ui->moveTopButton);
	ui->bhorWidget->layout()->addItem(ui->chorizontalSpacer_2);
	ui->bhorWidget->layout()->addWidget(ui->artistsButton);
	ui->bhorWidget->layout()->addItem(ui->chorizontalSpacer_3);
	ui->bhorWidget->layout()->addWidget(ui->moveDownButton);
	ui->bhorWidget->layout()->addItem(ui->chorizontalSpacer_4);
	ui->bhorWidget->layout()->addWidget(ui->sortAlphDescButton);
	ui->bhorWidget->layout()->addItem(ui->chorizontalSpacer_5);
	ui->bhorWidget->layout()->addWidget(ui->shuffleButton);

	ui->bhorWidget->show();
}

void PlaylistSortForm::portraitMode() {
	ui->topWidget->hide();
	ui->bottomWidget->hide();
	ui->bhorWidget->hide();

	ui->bhorWidget->layout()->removeWidget(ui->doneButton);
	ui->bhorWidget->layout()->removeItem(ui->chorizontalSpacer_0);
	ui->bhorWidget->layout()->removeWidget(ui->moveTopButton);
	ui->bhorWidget->layout()->removeItem(ui->chorizontalSpacer_1);
	ui->bhorWidget->layout()->removeWidget(ui->sortAlphAscButton);
	ui->bhorWidget->layout()->removeItem(ui->chorizontalSpacer_2);
	ui->bhorWidget->layout()->removeWidget(ui->artistsButton);
	ui->bhorWidget->layout()->removeItem(ui->chorizontalSpacer_3);
	ui->bhorWidget->layout()->removeWidget(ui->sortAlphDescButton);
	ui->bhorWidget->layout()->removeItem(ui->chorizontalSpacer_4);
	ui->bhorWidget->layout()->removeWidget(ui->moveDownButton);
	ui->bhorWidget->layout()->removeItem(ui->chorizontalSpacer_5);
	ui->bhorWidget->layout()->removeWidget(ui->shuffleButton);

	ui->topWidget->layout()->removeItem(ui->thorizontalSpacer_0);
	ui->topWidget->layout()->removeItem(ui->thorizontalSpacer_1);
	ui->topWidget->layout()->removeWidget(ui->sortAlphAscButton);
	ui->topWidget->layout()->removeWidget(ui->sortAlphDescButton);
	ui->topWidget->layout()->removeWidget(ui->artistsButton);

	ui->bottomWidget->layout()->removeItem(ui->bhorizontalSpacer_0);
	ui->bottomWidget->layout()->removeItem(ui->bhorizontalSpacer_1);
	ui->bottomWidget->layout()->removeItem(ui->bhorizontalSpacer_2);
	ui->bottomWidget->layout()->removeWidget(ui->doneButton);
	ui->bottomWidget->layout()->removeWidget(ui->moveDownButton);
	ui->bottomWidget->layout()->removeWidget(ui->moveTopButton);
	ui->bottomWidget->layout()->removeWidget(ui->shuffleButton);

	ui->topWidget->layout()->addWidget(ui->sortAlphAscButton);
	ui->topWidget->layout()->addItem(ui->thorizontalSpacer_0);
	ui->topWidget->layout()->addWidget(ui->artistsButton);
	ui->topWidget->layout()->addItem(ui->thorizontalSpacer_1);
	ui->topWidget->layout()->addWidget(ui->sortAlphDescButton);

	ui->bottomWidget->layout()->addWidget(ui->doneButton);
	ui->bottomWidget->layout()->addItem(ui->bhorizontalSpacer_0);
	ui->bottomWidget->layout()->addWidget(ui->moveDownButton);
	ui->bottomWidget->layout()->addItem(ui->bhorizontalSpacer_1);
	ui->bottomWidget->layout()->addWidget(ui->moveTopButton);
	ui->bottomWidget->layout()->addItem(ui->bhorizontalSpacer_2);
	ui->bottomWidget->layout()->addWidget(ui->shuffleButton);

	ui->topWidget->show();
	ui->bottomWidget->show();
}

void PlaylistSortForm::updateTranslations() {
	ui->retranslateUi(this);
}

void PlaylistSortForm::updateGradiend() {
	Config config;
	if (config.getValue("ui/gradient").toString() == "yes") {
		ui->bottomWidget->setStyleSheet(_bottom_gradient);
		ui->topWidget->setStyleSheet(_top_gradient);
	} else {
		ui->topWidget->setStyleSheet("");
		ui->bottomWidget->setStyleSheet("");
	}
}

void PlaylistSortForm::updateIcons() {
	Config config;
	QString icons_theme = config.getValue("ui/iconstheme").toString();
	ui->doneButton->setIcon(QIcon(":/icons/"+icons_theme+"/use.png"));
	ui->moveDownButton->setIcon(QIcon(":/icons/"+icons_theme+"/arrow_bottom.png"));
	ui->moveTopButton->setIcon(QIcon(":/icons/"+icons_theme+"/arrow_top.png"));
	ui->shuffleButton->setIcon(QIcon(":/icons/"+icons_theme+"/random_on.png"));
	ui->sortAlphAscButton->setIcon(QIcon(":/icons/"+icons_theme+"/sort_alpha_asc.png"));
	ui->sortAlphDescButton->setIcon(QIcon(":/icons/"+icons_theme+"/sort_alpha_desc.png"));
	ui->artistsButton->setIcon(QIcon(":/icons/"+icons_theme+"/artists.png"));
}
