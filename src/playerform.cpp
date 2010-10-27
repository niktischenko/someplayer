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
#include <QTime>
#include <QSlider>
#include "trackrenderer.h"
#include <QResource>
#include "playlistdialog.h"
#include "edittagsdialog.h"
#include <QSpacerItem>
#include "config.h"

using namespace SomePlayer::DataObjects;
using namespace SomePlayer::Playback;
using namespace SomePlayer::Storage;

inline void __fill_list(QStandardItemModel *_model, Playlist playlist) {
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
		_model->setItem(i, 0, new QStandardItem(""));
	}
}

PlayerForm::PlayerForm(Library* lib, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerForm)
{
	_lib = lib;
	Config config;
	_icons_theme = config.getValue("ui/iconstheme").toString();
	_player = new Player(this);
	_time = new QTime();
	ui->setupUi(this);
	if (_player->random()) {
		ui->randomButton->setIcon(QIcon(":/icons/"+_icons_theme+"/random_on.png"));
	} else {
		ui->randomButton->setIcon(QIcon(":/icons/"+_icons_theme+"/random_off.png"));
	}
	if (_player->repeat() == REPEAT_ALL) {
		ui->repeatButton->setIcon(QIcon(":/icons/"+_icons_theme+"/repeat_all.png"));
	} else if (_player->repeat() == REPEAT_NO){
		ui->repeatButton->setIcon(QIcon(":/icons/"+_icons_theme+"/repeat_off.png"));
	} else {
		ui->repeatButton->setIcon(QIcon(":/icons/"+_icons_theme+"/repeat_one.png"));
	}

	_fscreen_button = new QPushButton(this);
	_fscreen_button->setFlat(true);
	_fscreen_button->setIcon(QIcon(":/icons/"+_icons_theme+"/fullscreen.png"));
	_fscreen_button->setCheckable(true);
	_fscreen_button->setMinimumSize(70, 70);
	_fscreen_button->setMaximumSize(70, 70);
	_fscreen_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	_fscreen_button->hide();

	ui->volumeSlider->setMinimum(0);
	ui->volumeSlider->setMaximum(100);
	ui->volumeSlider->setValue(config.getValue("playback/volume").toInt());
	_player->setVolume(ui->volumeSlider->value());
	ui->volumeSlider->hide();
	ui->seekSlider->setEnabled(false);
	ui->progressLayout->removeItem(ui->seekSpacer);
	_tools_widget = new ToolsWidget(this);
	ui->toolsLayout->insertWidget(0, _tools_widget);
	_tools_widget->hide();
	_model = new QStandardItemModel(0, 2, this);
	ui->playlistView->setModel(_model);
	_context_menu = new QMenu(ui->playlistView);
	QAction *clear_playlist = _context_menu->addAction("Clear playlist");
	QAction *delete_action = _context_menu->addAction("Delete");
	QAction *add_to_favorites = _context_menu->addAction("Add to favorites");
	QAction *enqueue_action = _context_menu->addAction("Enqueue");
	QAction *add_to_playlists = _context_menu->addAction("Add to playlists");
	QAction *edit_tags = _context_menu->addAction("Edit tags");

	_track_renderer = new TrackRenderer(this);
	_track_renderer->setActiveRow(-1);
	_track_renderer->setSearchRow(-1);
	ui->playlistView->setItemDelegateForColumn(1, _track_renderer);
	ui->playlistView->setItemDelegateForColumn(0, _track_renderer);

	_tag_resolver = new TagResolver(this);

	connect(ui->libraryButton, SIGNAL(clicked()), this, SLOT(_library()));
	connect(ui->viewButton, SIGNAL(clicked()), this, SLOT(_toggle_view()));
	connect(ui->playlistView, SIGNAL(activated(QModelIndex)), this, SLOT(_process_click(QModelIndex)));
	connect(ui->playlistView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(_process_dbl_click(QModelIndex)));
	connect(ui->playpauseButton, SIGNAL(clicked()), _player, SLOT(toggle()));
	connect(ui->nextButton, SIGNAL(clicked()), _player, SLOT(next()));
	connect(ui->stopButton, SIGNAL(clicked()), _player, SLOT(stop()));
	connect(ui->prevButton, SIGNAL(clicked()), _player, SLOT(prev()));
	connect(_player, SIGNAL(trackChanged(Track)), this, SLOT(_track_changed(Track)));
	connect(_player, SIGNAL(tick(int,int)), this, SLOT(_tick(int,int)));
	connect(ui->randomButton, SIGNAL(clicked()), this, SLOT(_toggle_random()));
	connect(ui->repeatButton, SIGNAL(clicked()), this, SLOT(_toggle_repeat()));
	connect(ui->seekSlider, SIGNAL(sliderMoved(int)), _player, SLOT(seek(int)));
	connect(ui->volumeSlider, SIGNAL(sliderMoved(int)), _player, SLOT(setVolume(int)));
	connect(ui->playlistView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(_custom_context_menu_requested(QPoint)));
	connect(clear_playlist, SIGNAL(triggered()), this, SIGNAL(clearPlaylist()));
	connect(delete_action, SIGNAL(triggered()), this, SLOT(_delete_track()));
	connect(enqueue_action, SIGNAL(triggered()), this, SLOT(_enqueue_track()));
	connect(add_to_favorites, SIGNAL(triggered()), this, SLOT(_add_to_favorites()));
	connect(add_to_playlists, SIGNAL(triggered()), this, SLOT(_add_to_playlists()));
	connect(edit_tags, SIGNAL(triggered()), this, SLOT(_edit_tags()));
	connect(_player, SIGNAL(stateChanged(PlayerState)), this, SLOT(_state_changed(PlayerState)));
	connect(_player, SIGNAL(trackDone(Track)), _lib, SLOT(updateTrackCount(Track)));
	connect(_tag_resolver, SIGNAL(decoded(Track)), this, SLOT(_track_decoded(Track)));
	connect(ui->volumeButton, SIGNAL(clicked()), this, SLOT(_toggle_volume()));
	connect(ui->moreButton, SIGNAL(clicked()), this, SLOT(_tools_widget_toggle()));
	connect(_tools_widget, SIGNAL(search(QString)), this, SLOT(search(QString)));
	connect(_tools_widget, SIGNAL(nextSearch()), this, SLOT(nextItem()));
	connect(_tools_widget, SIGNAL(prevSearch()), this, SLOT(prevItem()));
	connect(_tools_widget, SIGNAL(toggleFullscreen(bool)), this, SIGNAL(fullscreen(bool)));
	connect(_tools_widget, SIGNAL(toggleFullscreen(bool)), _fscreen_button, SLOT(setChecked(bool)));
	connect(_fscreen_button, SIGNAL(clicked(bool)), this, SIGNAL(fullscreen(bool)));
	connect(_fscreen_button, SIGNAL(clicked(bool)), _tools_widget, SLOT(setFullscreenState(bool)));
	ui->viewButton->setIcon(QIcon(":/icons/"+_icons_theme+"/playback.png"));
	_top_gradient = ui->topWidget->styleSheet();
	_bottom_gradient = ui->bottomWidget->styleSheet();

	// dbus
	_dbusadaptor = new DBusAdaptop(_player);
	QDBusConnection connection = QDBusConnection::sessionBus();
	bool ret = connection.registerService(_SERVICE_NAME_);
	ret = connection.registerObject("/", _player);
}

PlayerForm::~PlayerForm()
{
    delete ui;
}

void PlayerForm::_library() {
	emit library();
}

void PlayerForm::reload(bool reread) {
	if (reread) {
		_current_playlist = _lib->getCurrentPlaylist();
		_player->setPlaylist(_current_playlist);
		__fill_list(_model, _current_playlist);
		ui->playlistView->setColumnWidth(0, 50);
	}
}

void PlayerForm::_toggle_view() {
	int index = ui->stackedWidget->currentIndex();
	index = (!index % 2);
	if (index) {
		ui->viewButton->setIcon(QIcon(":/icons/"+_icons_theme+"/playlist.png"));
		ui->moreButton->hide();
		_fscreen_button->show();
	} else {
		ui->viewButton->setIcon(QIcon(":/icons/"+_icons_theme+"/playback.png"));
		ui->moreButton->show();
		_fscreen_button->hide();
	}
	ui->stackedWidget->setCurrentIndex(index);
}

void PlayerForm::_process_click(QModelIndex index) {
	if (index.column() == 1) {
		int id = index.row();
		_player->stop();
		_player->setTrackId(id);
		_player->play();
		_track_renderer->setActiveRow(id);
	} else {
		_custom_context_menu_requested(ui->playlistView->rect().center());
	}
//	ui->playlistView->hide();
//	ui->playlistView->show();
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
	ui->seekSlider->setMinimum(0);
	ui->seekSlider->setMaximum(track.metadata().length());
	_tick(0, track.metadata().length());
}

void PlayerForm::_tick(int done, int all) {
	_time->setHMS(0, all/60, all%60);
	ui->allTimeLabel->setText(_time->toString("mm:ss"));
	_time->setHMS(0, done/60, done%60);
	ui->doneTimeLabel->setText(_time->toString("mm:ss"));
	ui->seekSlider->setValue(done);
}

void PlayerForm::_slider_released() {
	_player->seek(ui->seekSlider->value());
}

void PlayerForm::_custom_context_menu_requested(const QPoint &pos) {
	_context_menu->exec(pos);
}

void PlayerForm::_delete_track() {
	QList<QModelIndex> idx = ui->playlistView->selectionModel()->selectedIndexes();
	if (idx.isEmpty())
		return;
	int id = idx.first().row();
	int aid = _track_renderer->activeRow();
	if (aid > id) {
		_track_renderer->setActiveRow(aid-1);
	} else if (aid == id) {
		_track_renderer->setActiveRow(-1);
	}
	_current_playlist.removeTrackAt(id);
	_lib->saveCurrentPlaylist(_current_playlist);
	reload(true);
}

void PlayerForm::_enqueue_track() {
	QList<QModelIndex> idx = ui->playlistView->selectionModel()->selectedIndexes();
	if (idx.isEmpty())
		return;
	int id = idx.first().row();
	_player->enqueue(id);
}

void PlayerForm::_add_to_favorites() {
	QList<QModelIndex> idx = ui->playlistView->selectionModel()->selectedIndexes();
	if (idx.isEmpty())
		return;
	int id = idx.first().row();
	_lib->addToFavorites(_current_playlist.tracks().at(id));
}

void PlayerForm::_state_changed(PlayerState state) {
	if (state == PLAYER_PLAYING) {
		ui->playpauseButton->setIcon(QIcon(":/icons/"+_icons_theme+"/pause.png"));
		ui->seekSlider->setEnabled(true);
	} else {
		if (state == PLAYER_STOPPED) {
			ui->seekSlider->setValue(0);
			ui->doneTimeLabel->setText("00:00");
			ui->seekSlider->setEnabled(false);
		}
		ui->playpauseButton->setIcon(QIcon(":/icons/"+_icons_theme+"/play.png"));
	}
}

void PlayerForm::_toggle_random() {
	_player->toggleRandom();
	if (_player->random()) {
		ui->randomButton->setIcon(QIcon(":/icons/"+_icons_theme+"/random_on.png"));
	} else {
		ui->randomButton->setIcon(QIcon(":/icons/"+_icons_theme+"/random_off.png"));
	}
}

void PlayerForm::_toggle_repeat() {
	_player->toggleRepeat();
	if (_player->repeat() == REPEAT_ALL) {
		ui->repeatButton->setIcon(QIcon(":/icons/"+_icons_theme+"/repeat_all.png"));
	} else if (_player->repeat() == REPEAT_NO){
		ui->repeatButton->setIcon(QIcon(":/icons/"+_icons_theme+"/repeat_off.png"));
	} else {
		ui->repeatButton->setIcon(QIcon(":/icons/"+_icons_theme+"/repeat_one.png"));
	}
}

void PlayerForm::search(QString pattern) {
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
	ui->playlistView->setColumnWidth(0, 50);
	_lib->saveCurrentPlaylist(_current_playlist);
	_player->setPlaylist(_current_playlist);
}

void PlayerForm::_add_to_playlists() {
	QList<QModelIndex> idx = ui->playlistView->selectionModel()->selectedIndexes();
	if (idx.isEmpty())
		return;
	int id = idx.first().row();

	QList<QString> names = _lib->getPlaylistsNames();
	names.removeOne(_CURRENT_PLAYLIST_SUBST_);
	PlaylistDialog dialog(names, this);
	if (dialog.exec() == QDialog::Accepted) {
		QList<QString> selected = dialog.selected();
		foreach (QString name, selected) {
			Playlist pl = _lib->getPlaylist(name);
			pl.addTrack(_current_playlist.tracks().at(id));
			_lib->savePlaylist(pl);
		}
	}
}

void PlayerForm::_edit_tags() {
	QList<QModelIndex> idx = ui->playlistView->selectionModel()->selectedIndexes();
	if (idx.isEmpty())
		return;
	Track track = _current_playlist.tracks().at(idx.first().row());

	EditTagsDialog dialog(this);
	dialog.setTrackMetadata(track.metadata());
	if (dialog.exec() == QDialog::Accepted) {
		track.setMetadata(dialog.meta());
		_lib->updateTrackMetadata(track);
		reload(true);
		emit refreshLibrary();
	}
}

void PlayerForm::stop() {
	_player->stop();
}

void PlayerForm::_toggle_volume() {
	if (ui->volumeSlider->isVisible()) {
		ui->volumeSlider->hide();
	} else {
		ui->volumeSlider->show();
		ui->volumeSlider->setValue(_player->volume());
	}
}

void PlayerForm::_volume_changed() {
	int value = ui->volumeSlider->value();
	_player->setVolume(value);
}

void PlayerForm::landscapeMode() {
	ui->progressLayout->removeItem(ui->seekSpacer);
	ui->progressLayout->insertWidget(1, ui->seekSlider);
	ui->progressWidget->setVisible(false);

	landscape = true;

	ui->topWidget->hide();
	ui->bottomWidget->hide();

	ui->bhorizontalLayout->removeItem(ui->chorizontalSpacer_0);
	ui->bhorizontalLayout->removeItem(ui->chorizontalSpacer_1);
	ui->bhorizontalLayout->removeItem(ui->chorizontalSpacer_2);
	ui->bhorizontalLayout->removeItem(ui->chorizontalSpacer_3);
	ui->bhorizontalLayout->removeItem(ui->chorizontalSpacer_4);
	ui->bhorizontalLayout->addWidget(ui->libraryButton);
	ui->bhorizontalLayout->addItem(ui->chorizontalSpacer_0);
	ui->bhorizontalLayout->addWidget(ui->viewButton);
	ui->bhorizontalLayout->addItem(ui->chorizontalSpacer_1);
	ui->bhorizontalLayout->addWidget(ui->randomButton);
	ui->bhorizontalLayout->addWidget(ui->repeatButton);
	ui->bhorizontalLayout->addItem(ui->chorizontalSpacer_2);
	ui->bhorizontalLayout->addWidget(ui->prevButton);
	ui->bhorizontalLayout->addWidget(ui->playpauseButton);
	ui->bhorizontalLayout->addWidget(ui->nextButton);
	ui->bhorizontalLayout->addWidget(ui->stopButton);
	ui->bhorizontalLayout->addItem(ui->chorizontalSpacer_3);
	ui->bhorizontalLayout->addWidget(ui->moreButton);
	ui->bhorizontalLayout->addWidget(_fscreen_button);
	ui->bhorizontalLayout->addItem(ui->chorizontalSpacer_4);
	ui->bhorizontalLayout->addWidget(ui->volumeButton);

	if (_tools_widget->isVisible()) {
		ui->moreButton->setIcon(QIcon(":/icons/"+_icons_theme+"/more.png"));
	} else {
		ui->moreButton->setIcon(QIcon(":/icons/"+_icons_theme+"/unmore.png"));
	}
}

void PlayerForm::portraitMode() {
	ui->progressLayout->insertSpacerItem(1, ui->seekSpacer);
	ui->progressWidget->layout()->addWidget(ui->seekSlider);
	ui->progressWidget->setVisible(true);

	ui->topWidget->show();
	ui->bottomWidget->show();

	landscape = false;

	ui->topWidget->layout()->removeItem(ui->thorizontalSpacer_0);
	ui->topWidget->layout()->removeItem(ui->thorizontalSpacer_1);
	ui->topWidget->layout()->removeItem(ui->thorizontalSpacer_2);
	ui->topWidget->layout()->removeItem(ui->thorizontalSpacer_3);
	ui->topWidget->layout()->addWidget(ui->prevButton);
	ui->topWidget->layout()->addItem(ui->thorizontalSpacer_0);
	ui->topWidget->layout()->addWidget(ui->stopButton);
	ui->topWidget->layout()->addItem(ui->thorizontalSpacer_1);
	ui->topWidget->layout()->addWidget(ui->playpauseButton);
	ui->topWidget->layout()->addItem(ui->thorizontalSpacer_2);
	ui->topWidget->layout()->addWidget(ui->nextButton);
	ui->topWidget->layout()->addItem(ui->thorizontalSpacer_3);
	ui->topWidget->layout()->addWidget(ui->moreButton);
	ui->topWidget->layout()->addWidget(_fscreen_button);
	ui->bhorizontalLayout->removeItem(ui->chorizontalSpacer_0);
	ui->bhorizontalLayout->removeItem(ui->chorizontalSpacer_1);
	ui->bhorizontalLayout->removeItem(ui->chorizontalSpacer_2);
	ui->bhorizontalLayout->removeItem(ui->chorizontalSpacer_3);
	ui->bhorizontalLayout->removeItem(ui->chorizontalSpacer_4);

	ui->bottomWidget->layout()->removeItem(ui->bhorizontalSpacer_0);
	ui->bottomWidget->layout()->removeItem(ui->bhorizontalSpacer_1);
	ui->bottomWidget->layout()->removeItem(ui->bhorizontalSpacer_2);
	ui->bottomWidget->layout()->removeItem(ui->bhorizontalSpacer_3);
	ui->bottomWidget->layout()->addWidget(ui->libraryButton);
	ui->bottomWidget->layout()->addItem(ui->bhorizontalSpacer_0);
	ui->bottomWidget->layout()->addWidget(ui->viewButton);
	ui->bottomWidget->layout()->addItem(ui->bhorizontalSpacer_1);
	ui->bottomWidget->layout()->addWidget(ui->randomButton);
	ui->bottomWidget->layout()->addItem(ui->bhorizontalSpacer_2);
	ui->bottomWidget->layout()->addWidget(ui->repeatButton);
	ui->bottomWidget->layout()->addItem(ui->bhorizontalSpacer_3);
	ui->bottomWidget->layout()->addWidget(ui->volumeButton);

	if (_tools_widget->isVisible()) {
		ui->moreButton->setIcon(QIcon(":/icons/"+_icons_theme+"/unmore.png"));
	} else {
		ui->moreButton->setIcon(QIcon(":/icons/"+_icons_theme+"/more.png"));
	}
}

void PlayerForm::_tools_widget_toggle() {
	if (_tools_widget->isVisible()) {
		ui->moreButton->setIcon(QIcon(landscape ? ":/icons/"+_icons_theme+"/unmore.png" : ":/icons/"+_icons_theme+"/more.png"));
		_tools_widget->hide();
		_tools_widget->reset();
		cancelSearch();
	} else {
		ui->moreButton->setIcon(QIcon(landscape ? ":/icons/"+_icons_theme+"/more.png" : ":/icons/"+_icons_theme+"/unmore.png"));
		_tools_widget->show();
		_tools_widget->setFocus();
	}
}

void PlayerForm::updateIcons() {
	Config config;
	_icons_theme = config.getValue("ui/iconstheme").toString();
	_tools_widget->updateIcons();
	ui->libraryButton->setIcon(QIcon(":/icons/"+_icons_theme+"/library.png"));
	if (_tools_widget->isVisible()) {
		ui->moreButton->setIcon(QIcon(landscape ? ":/icons/" + _icons_theme + "/unmore.png" : ":/icons/" + _icons_theme + "/more.png"));
	} else {
		ui->moreButton->setIcon(QIcon(landscape ? ":/icons/" + _icons_theme + "/more.png" : ":/icons/" + _icons_theme + "/unmore.png"));
	}
	ui->nextButton->setIcon(QIcon(":/icons/"+_icons_theme+"/next.png"));
	ui->stopButton->setIcon(QIcon(":/icons/"+_icons_theme+"/stop.png"));
	ui->prevButton->setIcon(QIcon(":/icons/"+_icons_theme+"/prev.png"));
	ui->volumeButton->setIcon(QIcon(":/icons/"+_icons_theme+"/volume.png"));
	if (_player->state() == PLAYER_PLAYING) {
		ui->playpauseButton->setIcon(QIcon(":/icons/"+_icons_theme+"/pause.png"));
	} else {
		ui->playpauseButton->setIcon(QIcon(":/icons/"+_icons_theme+"/play.png"));
	}
	if (ui->stackedWidget->currentIndex()) {
		ui->viewButton->setIcon(QIcon(":/icons/"+_icons_theme+"/playlist.png"));
	} else {
		ui->viewButton->setIcon(QIcon(":/icons/"+_icons_theme+"/playback.png"));
	}
	if (_player->repeat() == REPEAT_ALL) {
		ui->repeatButton->setIcon(QIcon(":/icons/"+_icons_theme+"/repeat_all.png"));
	} else if (_player->repeat() == REPEAT_NO){
		ui->repeatButton->setIcon(QIcon(":/icons/"+_icons_theme+"/repeat_off.png"));
	} else {
		ui->repeatButton->setIcon(QIcon(":/icons/"+_icons_theme+"/repeat_one.png"));
	}
	if (_player->random()) {
		ui->randomButton->setIcon(QIcon(":/icons/"+_icons_theme+"/random_on.png"));
	} else {
		ui->randomButton->setIcon(QIcon(":/icons/"+_icons_theme+"/random_off.png"));
	}
}

void PlayerForm::checkGradient() {
	Config config;
	if (config.getValue("ui/gradient").toString() == "yes") {
		ui->bottomWidget->setStyleSheet(_bottom_gradient);
		ui->topWidget->setStyleSheet(_top_gradient);
	} else {
		ui->topWidget->setStyleSheet("");
		ui->bottomWidget->setStyleSheet("");
	}
}

void PlayerForm::play(Track track) {
	reload(true);
	int id = _current_playlist.tracks().indexOf(track);
	if (id >= 0) {
		_player->setTrackId(id);
		_player->play();
	}
}

void PlayerForm::_process_dbl_click(QModelIndex) {
	_custom_context_menu_requested(ui->playlistView->rect().center());
}
