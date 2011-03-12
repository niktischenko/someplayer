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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QFile>
#include <QDesktopWidget>
#include <QTranslator>
#include <QKeyEvent>

#include "player/player.h"

#include "library.h"
#include "timerdialog.h"
#include "equalizerdialog.h"
#include "saveplaylistdialog.h"
#include "settingsform.h"

using namespace SomePlayer::DataObjects;
using namespace SomePlayer::Storage;
using namespace SomePlayer::Playback;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	_display_unlocked = true; // in most cases
	Config config;
	_library = new Library(config.applicationDir(), config.applicationDir());
	_translator = new QTranslator(this);
	ui->setupUi(this);
	connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
	connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(settings()));
	setAnimated(true);
	setAttribute(Qt::WA_Maemo5StackedWindow);
	_player_form = new PlayerForm(_library, this);
	ui->centralWidget->layout()->addWidget(_player_form);
	_library_form = new LibraryForm(_library, this);
	_directory_form = new DirectoryView(this);
	_directory_form->hide();
	_timer = new QTimer(this);
	_equalizer_dialog = new EqualizerDialog(this);
	_manage_library_form = new ManageLibraryForm(_library, this);
	_settings_form = new SettingsForm(this);
	_settings_form->hide();
	_about_form = new AboutForm(this);
	_about_form->hide();
	connect(_player_form, SIGNAL(library()), this, SLOT(library()));
	connect(_library_form, SIGNAL(refreshPlayer()), this, SLOT(player()));
	connect(ui->actionManageLibrary, SIGNAL(triggered()), this, SLOT(_manage_library()));
	connect(ui->actionSavePlaylist, SIGNAL(triggered()), this, SLOT(_save_playlist()));
	connect(_player_form, SIGNAL(clearPlaylist()), this, SLOT(_clear_current_playlist()));
	connect(ui->actionSetTimer, SIGNAL(triggered()), this, SLOT(_set_timer()));
	connect(ui->actionEqualizer, SIGNAL(triggered()), this, SLOT(_equalizer()));
	connect(ui->actionOnlineHelp, SIGNAL(triggered()), _about_form, SLOT(onlineHelp()));
	connect(_library, SIGNAL(done()), _library_form, SLOT(refresh()));
	connect(_player_form, SIGNAL(refreshLibrary()), _library_form, SLOT(refresh()));
	connect(_manage_library_form, SIGNAL(refreshLibrary()), _library_form, SLOT(refresh()));
	connect(_timer, SIGNAL(timeout()), this, SLOT(_timeout()));
	connect(_equalizer_dialog, SIGNAL(valueChanged(int,int)), this, SLOT(_equalizer_value_changed(int, int)));
	connect(_equalizer_dialog, SIGNAL(equalizerEnabled()), _player_form, SLOT(enableEqualizer()));
	connect(_equalizer_dialog, SIGNAL(equalizerDisabled()), _player_form, SLOT(disableEqualizer()));
	connect(QApplication::desktop(), SIGNAL(resized(int)), this, SLOT(_orientation_changed()));
	connect(_player_form, SIGNAL(fullscreen(bool)), this, SLOT(_fullscreen(bool)));
	connect(_library_form, SIGNAL(addAndPlay(Track)), _player_form, SLOT(play(Track)));
	connect(_directory_form, SIGNAL(addAndPlay(Track)), _player_form, SLOT(play(Track)));
	connect(_player_form, SIGNAL(dirView()), _directory_form, SLOT(show()));
	connect(_directory_form, SIGNAL(addTracks(QList<Track>)), this, SLOT(_add_tracks(QList<Track>)));
	connect(_settings_form, SIGNAL(iconsChanged()), _player_form, SLOT(updateIcons()));
	connect(_settings_form, SIGNAL(iconsChanged()), _library_form, SLOT(updateIcons()));
	connect(_settings_form, SIGNAL(iconsChanged()), _manage_library_form, SLOT(updateIcons()));
	connect(_settings_form, SIGNAL(iconsChanged()), _directory_form, SLOT(updateIcons()));
	connect(_settings_form, SIGNAL(iconsChanged()), _about_form, SLOT(updateIcons()));
	connect(_settings_form, SIGNAL(gradientChanged()), _player_form, SLOT(checkGradient()));
	connect(_settings_form, SIGNAL(gradientChanged()), _library_form, SLOT(checkGradient()));
	connect(_settings_form, SIGNAL(gradientChanged()), _directory_form, SLOT(checkGradient()));
	connect(_settings_form, SIGNAL(libraryOptionsChanged()), _library_form, SLOT(refresh()));
	connect(_settings_form, SIGNAL(orientationChanged()), this, SLOT(_change_orientation()));
	connect(_settings_form, SIGNAL(translationChanged()), this, SLOT(updateTranslations()));
	connect(_settings_form, SIGNAL(trackColorChanged()), _player_form, SLOT(updateTrackColor()));
	connect(_settings_form, SIGNAL(hwZoomPolicyChanged()), this, SLOT(_hw_zoom_policy_changed()));
	connect(&_dbus_client, SIGNAL(displayStateChanged(bool)), this, SLOT(_set_display_state(bool)));
	connect(_settings_form, SIGNAL(fmtxSettingsChanged()), this, SLOT(_fmtx_settings_changed()));
	_player_form->reload(true);
	QString mode = config.getValue("ui/orientation").toString();
	if (mode == "landscape") {
		setAttribute(Qt::WA_Maemo5LandscapeOrientation);
		_player_form->landscapeMode();
		_library_form->landscapeMode();
		_equalizer_dialog->landscapeMode();
		_directory_form->lanscapeMode();
		_settings_form->landscapeMode();
	} else if (mode == "portrait") {
		setAttribute(Qt::WA_Maemo5PortraitOrientation);
		_player_form->portraitMode();
		_library_form->portraitMode();
		_equalizer_dialog->portraitMode();
		_directory_form->portraitMode();
		_settings_form->portraitMode();
	} else if (mode == "auto") { // initialization in landscape
		_player_form->landscapeMode();
		_library_form->landscapeMode();
		_equalizer_dialog->landscapeMode();
		_directory_form->lanscapeMode();
		_settings_form->landscapeMode();
		setAttribute(Qt::WA_Maemo5AutoOrientation);
	}
	_library_form->updateIcons();
	_player_form->updateIcons();
	_manage_library_form->updateIcons();
	_directory_form->updateIcons();
	_player_form->checkGradient();
	_library_form->checkGradient();
	_directory_form->checkGradient();
	_hw_zoom_policy_changed();
	config.setValue("fmtx/enabled", "no");
	setWindowTitle("SomePlayer");

	QList<QWidget *> widgets = ui->centralWidget->findChildren<QWidget *>();
	foreach (QWidget *widget, widgets) {
		if (widget->objectName() != "searchLine") {
			widget->installEventFilter(this);
		}
	}
}

MainWindow::~MainWindow()
{
	delete _player_form;
	delete _library_form;
	delete ui;
}

void MainWindow::about() {
	_about_form->show();
}

void MainWindow::player() {
	_player_form->reload(true);
	_orientation_changed(); // workaround
}

void MainWindow::library() {
	ui->menuBar->setEnabled(true);
	_library_form->show();
	_orientation_changed(); // workaround
	_manage_library_form->hide();
}

void MainWindow::_manage_library() {
	_manage_library_form->refresh();
	_manage_library_form->show();
}

void MainWindow::_save_playlist() {
	QList<QString> playlists = _library->getPlaylistsNames();
	playlists.removeOne(_CURRENT_PLAYLIST_SUBST_);
	Playlist cur = _library->getCurrentPlaylist();
	// construct playlist name if possible
	QString suggest_name;
	QList<Track> tracks = cur.tracks();
	QString artist = tracks.at(0).metadata().artist(), album = tracks.at(0).metadata().album();
	foreach (Track t, tracks) {
		if (t.metadata().album() != album)
			album = "";
		if (t.metadata().artist() != artist)
			artist = "";
	}
	if (album.isEmpty() && artist.isEmpty()) {
		suggest_name = "New playlist";
	} else if (album.isEmpty()) {
		suggest_name = artist;
	} else {
		suggest_name = QString("%1 - %2").arg(artist).arg(album);
	}

	// constructed
	SavePlaylistDialog dialog(suggest_name, this);
	dialog.setPlaylistNames(playlists);
	if (dialog.exec() == QDialog::Accepted) {
		QString name = dialog.selectedName();
		bool append = false;
		if (playlists.contains(name)) {
			if (QMessageBox::question(this, "Overwrite playlist?", "Overwrite playlist \""+name+"\"?", QMessageBox::Ok, QMessageBox::Cancel)
				!= QMessageBox::Ok) {
				return;
			}
			append = (QMessageBox::question(this, "Append to playlist?", "Playlist with name \""+name+"\" already exists.\n"
						  "Dow you want to append current playlist to it?",
						  QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok);
		}
		if (append) {
			Playlist target = _library->getPlaylist(name);
			QList<Track> tracks = cur.tracks();
			foreach (Track track, tracks) {
				target.addTrack(track);
			}
			_library->savePlaylist(target);
		} else {
			cur.setName(name);
			_library->savePlaylist(cur);
		}
	}
}

void MainWindow::_clear_current_playlist() {
	CONFIRM_ACTION(this, tr("Clear playlist?"))
	Playlist playlist = _library->getCurrentPlaylist();
	playlist.clear();
	_library->saveCurrentPlaylist(playlist);
	_player_form->reload(true);
}

void MainWindow::_set_timer() {
	TimerDialog dialog(this);
	dialog.init();
	if (_timer->isActive()) {
		dialog.showDisable();
		int msec = _timeout_interval;
		int h = msec/3600000;
		int m = msec/60000 - h * 60;
		int s = msec/1000 - h * 3600 - m * 60;
		dialog.setTime(h, m, s);
	}
	if (QDialog::Accepted == dialog.exec()) {
		if (!dialog.timerDisabled()) {
			int h, m, s;
			dialog.getTime(&h, &m, &s);
			_timeout_interval = h*3600000+m*60000+s*1000;
			_timer->setInterval(1000);
			_timer->setSingleShot(false);
			_timer->start();
		} else if (_timer->isActive()) {
			_timer->stop();
			_player_form->hideCountdown();
		}
	}
}

void MainWindow::_timeout() {
	_timeout_interval -= 1000;
	if (_timeout_interval <= 0) {
		_player_form->stop();
		_player_form->hideCountdown();
		_timer->stop();
	} else {
		int h = _timeout_interval / 3600000;
		int m = (_timeout_interval / 60000) - 60*h;
		int s = (_timeout_interval / 1000) - 3600*h - 60*m;
		QString hp = h < 10 ? QString("0%1").arg(h) : QString("%1").arg(h);
		QString mp = m < 10 ? QString("0%1").arg(m) : QString("%1").arg(m);
		QString sp = s < 10 ? QString("0%1").arg(s) : QString("%1").arg(s);
		_player_form->showCountdown(tr("Music off: ")+hp+":"+mp+":"+sp);
	}
}

void MainWindow::_equalizer() {
	if (_player_form->isEqualizerAvailable()) {
		double val = 0;
		for (int i = 0; i < 10; i++) {
			_player_form->equalizerValue(i, &val);
			_equalizer_dialog->setValue(i, (int)(val * 10 + 0.5));
		}
		_equalizer_dialog->setEqualizerEnabled(_player_form->isEqualizerEnabled());
		_equalizer_dialog->reloadPresets();
		_equalizer_dialog->exec();
	} else {
		QMessageBox::information(this, "Error", "No equalizer support. Please install gstreamer0.10-plugins-good-extra");
	}
}

void MainWindow::_equalizer_value_changed(int band, int val) {
	_player_form->setEqualizerValue(band, (val / 10.0));
}

void MainWindow::settings() {
	_settings_form->show();
}

void MainWindow::_change_orientation() {
	Config config;
	QString mode = config.getValue("ui/orientation").toString();
	if (mode == "landscape") {
		setAttribute(Qt::WA_Maemo5LandscapeOrientation);
	} else if (mode == "portrait") {
		setAttribute(Qt::WA_Maemo5PortraitOrientation);
	} else if (mode == "auto") {
		setAttribute(Qt::WA_Maemo5AutoOrientation);
	}
}

void MainWindow::_orientation_changed() {
	QRect screenGeometry = QApplication::desktop()->screenGeometry();
	if (screenGeometry.width() > screenGeometry.height()) {
		_orientation = ORIENTATION_LANDSCAPE;
		_player_form->landscapeMode();
		_library_form->landscapeMode();
		_equalizer_dialog->landscapeMode();
		_directory_form->lanscapeMode();
		_settings_form->landscapeMode();
	} else {
		_orientation = ORIENTATION_PORTRAIT;
		_player_form->portraitMode();
		_library_form->portraitMode();
		_equalizer_dialog->portraitMode();
		_directory_form->portraitMode();
		_settings_form->portraitMode();
	}
}

void MainWindow::_fullscreen(bool f) {
	if (f) showFullScreen();
	else showNormal();
}

void MainWindow::_add_tracks(QList<Track> tracks) {
	Playlist cur = _library->getCurrentPlaylist();
	foreach (Track track, tracks) {
		cur.addTrack(track);
	}
	_library->saveCurrentPlaylist(cur);
	_player_form->reload(true);
}

void MainWindow::updateTranslations() {
	Config config;
	if (config.getValue("ui/language").toString() != "en") {
		_translator->load(QString("/opt/someplayer/someplayer_%1").arg(config.getValue("ui/language").toString()));
		QApplication::installTranslator(_translator);
	} else {
		QApplication::removeTranslator(_translator);
	}
	ui->retranslateUi(this);
	_player_form->updateTranslations();
	_library_form->updateTranslations();
	_equalizer_dialog->updateTranslations();
	_manage_library_form->updateTranslations();
	_directory_form->updateTranslations();
	_settings_form->updateTranslations();
	_about_form->updateTranslations();
}

void MainWindow::_hw_zoom_policy_changed() {
	Config config;
	QString state = config.getValue("hw/zoomkeys").toString();
	if (state == "enabled") {
		_dbus_client.enableKeys();
		connect(&_dbus_client, SIGNAL(zoomKeyPressed(quint32)), this, SLOT(_zoom_key_pressed(quint32)));
	} else {
		_dbus_client.disableKeys();
		disconnect(&_dbus_client, SIGNAL(zoomKeyPressed(quint32)), this, SLOT(_zoom_key_pressed(quint32)));
	}
}

void MainWindow::_set_display_state(bool state) {
	_display_unlocked = state;
	if (!_display_unlocked) { // remember volume level when blocking screen
		_system_volume = _dbus_client.getVolume();
	}
}

void MainWindow::_zoom_key_pressed(quint32 code) {
	if (_display_unlocked) {
		return;
	}
	Config config;
	QString behavior = config.getValue("hw/zoom_action").toString();
	bool inverted = config.getValue("hw/zoom_inverted").toBool();
	if (inverted) {
		if (code == MM_KEY_DOWN) {
			code = MM_KEY_UP;
		} else {
			code = MM_KEY_DOWN;
		}
	}
	if (code == MM_KEY_DOWN) {
		if (behavior == "track") {
			if (_orientation == ORIENTATION_LANDSCAPE) {
				_player_form->next();
			} else {
				_player_form->prev();
			}
			_dbus_client.setVolume(_system_volume);
		}
	} else if (code == MM_KEY_UP) {
		if (behavior == "track") {
			if (_orientation == ORIENTATION_LANDSCAPE) {
				_player_form->prev();
			} else {
				_player_form->next();
			}
			_dbus_client.setVolume(_system_volume);
		}
	}
}

void MainWindow::_fmtx_settings_changed() {
	Config config;
	if (config.getValue("fmtx/enabled").toString() == "yes") {
		QString station_name = config.getValue("fmtx/station_name").toString();
		int frequency = config.getValue("fmtx/frequency").toInt();
		system(QString("fmtx_client -p1 -f%1 -s\"%2\" -t\"%3\" 2>&1 >/dev/null")
		       .arg(frequency)
		       .arg(station_name)
		       .arg(_player_form->playerCaption()).toAscii());
	} else {
		system("fmtx_client -p 0 2>&1 >/dev/null");
	}
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
	if (event->type() != QEvent::KeyPress) {
		return QObject::eventFilter(obj, event);
	}
	QKeyEvent *keyEvent = (QKeyEvent *) event;
	int mod = keyEvent->modifiers();

	switch (keyEvent->key()) {
	case Qt::Key_Space:
		if ((mod & Qt::ControlModifier) == 0) {
			_player_form->toggle();
		}
		break;
	case Qt::Key_Enter:
		// TODO
		break;
	case Qt::Key_Right:
		_player_form->next();
		break;
	case Qt::Key_Left:
		_player_form->prev();
		break;
	case Qt::Key_T:
		_player_form->stop();
		break;
	case Qt::Key_R:
		_player_form->toggleRandom();
		break;
	case Qt::Key_V:
		_player_form->toggleView();
		break;
	case Qt::Key_M:
		_directory_form->show();
		break;
	case Qt::Key_L:
		_library_form->show();
		break;
	case Qt::Key_E:
		_player_form->toggleRepeat();
		break;
	case Qt::Key_S:
		_player_form->toggleToolsWidget();
		break;
	case Qt::Key_H:
		_about_form->onlineHelp();
		break;
	default:
		return QObject::eventFilter(obj, event);
	}

	return true;
}
