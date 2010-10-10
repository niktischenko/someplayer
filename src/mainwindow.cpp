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

#include "player/player.h"

#include "library.h"
#include "timerdialog.h"
#include "equalizerdialog.h"
#include "saveplaylistdialog.h"
#include "settingsdialog.h"

using namespace SomePlayer::DataObjects;
using namespace SomePlayer::Storage;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	Config config;
	_library = new Library(config.applicationDir(), config.applicationDir());
	ui->setupUi(this);
	connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
	connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(settings()));
	setAnimated(true);
	_player_form = new PlayerForm(_library, ui->stackedWidget);
	_library_form = new LibraryForm(_library, ui->stackedWidget);
	_busy_widget = new BusyWidget(ui->stackedWidget);
	_timer = new QTimer(this);
	_equalizer_dialog = new EqualizerDialog(this);
	ui->stackedWidget->insertWidget(0, _player_form);
	ui->stackedWidget->insertWidget(1, _library_form);
	ui->stackedWidget->insertWidget(2, _busy_widget);
	QAction *add_directory = ui->menuLibrary->addAction("Add directory");
	QAction *save_playlist = ui->menuLibrary->addAction("Save playlist");
	QAction *clear_playlist = ui->menuLibrary->addAction("Clear current playlist");
	QAction *add_files = ui->menuLibrary->addAction("Add file to current playlist");
	QAction *set_timer = ui->menuBar->addAction("Set timer");
	QAction *equalizer = ui->menuBar->addAction("Equalizer");
	connect(_player_form, SIGNAL(library()), this, SLOT(library()));
	connect(_library_form, SIGNAL(player(bool)), this, SLOT(player(bool)));
	connect(add_directory, SIGNAL(triggered()), this, SLOT(_add_directory()));
	connect(save_playlist, SIGNAL(triggered()), this, SLOT(_save_playlist()));
	connect(clear_playlist, SIGNAL(triggered()), this, SLOT(_clear_current_playlist()));
	connect(add_files, SIGNAL(triggered()), this, SLOT(_add_files()));
	connect(set_timer, SIGNAL(triggered()), this, SLOT(_set_timer()));
	connect(equalizer, SIGNAL(triggered()), this, SLOT(_equalizer()));
	connect(_library, SIGNAL(done()), this, SLOT(library()));
	connect(_library, SIGNAL(done()), _library_form, SLOT(refresh()));
	connect(_library, SIGNAL(addingTracks(int)), _busy_widget, SLOT(setMax(int)));
	connect(_library, SIGNAL(trackAdded()), _busy_widget, SLOT(tick()));
	connect(_library_form, SIGNAL(done()), this, SLOT(library()));
	connect(_library_form, SIGNAL(busy(QString)), this, SLOT(showBusyWidget(QString)));
	connect(_timer, SIGNAL(timeout()), this, SLOT(_timeout()));
	connect(_equalizer_dialog, SIGNAL(valueChanged(int,int)), this, SLOT(_equalizer_value_changed(int, int)));
	connect(_equalizer_dialog, SIGNAL(equalizerEnabled()), _player_form, SLOT(enableEqualizer()));
	connect(_equalizer_dialog, SIGNAL(equalizerDisabled()), _player_form, SLOT(disableEqualizer()));
	connect(QApplication::desktop(), SIGNAL(resized(int)), this, SLOT(_orientation_changed()));
	connect(_player_form, SIGNAL(fullscreen(bool)), this, SLOT(_fullscreen(bool)));
	connect(_library_form, SIGNAL(fullscreen(bool)), this, SLOT(_fullscreen(bool)));
	_player_form->reload(true);
	library();
	QString mode = config.getValue("ui/orientation").toString();
	if (mode == "landscape") {
		setAttribute(Qt::WA_Maemo5LandscapeOrientation);
		_player_form->landscapeMode();
		_library_form->landscapeMode();
		_equalizer_dialog->landscapeMode();
	} else if (mode == "portrait") {
		setAttribute(Qt::WA_Maemo5PortraitOrientation);
		_player_form->portraitMode();
		_library_form->portraitMode();
		_equalizer_dialog->portraitMode();
	} else if (mode == "auto") { // initialization in landscape
		_player_form->landscapeMode();
		_library_form->landscapeMode();
		_equalizer_dialog->landscapeMode();
		setAttribute(Qt::WA_Maemo5AutoOrientation);
	}
	_library_form->updateIcons();
	_player_form->updateIcons();
	_player_form->checkGradient();
	_library_form->checkGradient();
}

MainWindow::~MainWindow()
{
	delete _player_form;
	delete _library_form;
	delete ui;
}

void MainWindow::about() {
	QMessageBox::about(this, QString("About SomePlayer v")+_SOMEPLAYER_VERSION_, "Alternate music player for Maemo 5 "
					   "written in C++ with Qt4\n\n"
					   "Author: Nikolay Tischenko aka \"somebody\" <niktischenko@gmail.com>");
}

void MainWindow::player(bool reread) {
	ui->stackedWidget->setCurrentIndex(0);
	_player_form->reload(reread);
	setWindowTitle("SomePlayer");
	_orientation_changed(); // workaround
}

void MainWindow::library() {
	ui->menuBar->setEnabled(true);
	ui->stackedWidget->setCurrentIndex(1);
	setWindowTitle("SomePlayer Library");
	_orientation_changed(); // workaround
}

void MainWindow::_add_directory() {
	QString directory = QFileDialog::getExistingDirectory (this, "Select directory", "/home/user/MyDocs", QFileDialog::ShowDirsOnly );
	if (!directory.isEmpty()) {
		showBusyWidget("<H1>Scanning... Please wait</H1>");
		_library->addDirectory(directory);
	}
}

void MainWindow::_save_playlist() {
	QList<QString> playlists = _library->getPlaylistsNames();
	playlists.removeOne(_CURRENT_PLAYLIST_SUBST_);
	SavePlaylistDialog dialog(this);
	dialog.setPlaylistNames(playlists);
	if (dialog.exec() == QDialog::Accepted) {
		QString name = dialog.selectedName();
		bool append = false;
		if (playlists.contains(name)) {
			if (QMessageBox::question(this, "Append to playlist?", "Playlist with name \""+name+"\" already exists.\n"
						  "Dow you want to append current playlist to it?",
						  QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok) {
				append = true;
			} else {
				append = false;
			}
		}
		if (append) {
			Playlist cur = _library->getCurrentPlaylist();
			Playlist target = _library->getPlaylist(name);
			QList<Track> tracks = cur.tracks();
			foreach (Track track, tracks) {
				target.addTrack(track);
			}
			_library->savePlaylist(target);
		} else {
			Playlist playlist = _library->getCurrentPlaylist();
			playlist.setName(name);
			_library->savePlaylist(playlist);
		}
	}
}

void MainWindow::_clear_current_playlist() {
	Playlist playlist = _library->getCurrentPlaylist();
	playlist.clear();
	_library->saveCurrentPlaylist(playlist);
	_player_form->reload(true);
}

void MainWindow::showBusyWidget(QString caption) {
	_busy_widget->setText(caption);
	ui->menuBar->setEnabled(false);
	ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::_add_files() {
	QStringList files = QFileDialog::getOpenFileNames(this, "Add file");
	if (!files.isEmpty()) _player_form->addFiles(files);
}

void MainWindow::_set_timer() {
	TimerDialog dialog(this);
	dialog.init();
	if (_timer->isActive()) {
		dialog.showDisable();
		int msec = _timer->interval();
		int h = msec/3600000;
		int m = msec/60000 - h * 60;
		int s = msec/1000 - h * 3600 - m * 60;
		dialog.setTime(h, m, s);
	}
	if (QDialog::Accepted == dialog.exec()) {
		if (!dialog.timerDisabled()) {
			int h, m, s;
			dialog.getTime(&h, &m, &s);
			_timer->setInterval(h*3600000+m*60000+s*1000);
			_timer->start();
		} else if (_timer->isActive()) {
			_timer->stop();
		}
	}
}

void MainWindow::_timeout() {
	_player_form->stop();
	_timer->stop();
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
	SettingsDialog dialog;
	dialog.exec();
	Config config;
	_library_form->refresh();
	QString mode = config.getValue("ui/orientation").toString();
	if (mode == "landscape") {
		setAttribute(Qt::WA_Maemo5LandscapeOrientation);
	} else if (mode == "portrait") {
		setAttribute(Qt::WA_Maemo5PortraitOrientation);
	} else if (mode == "auto") {
		setAttribute(Qt::WA_Maemo5AutoOrientation);
	}
	_player_form->updateIcons();
	_library_form->updateIcons();
	_player_form->checkGradient();
	_library_form->checkGradient();
}

void MainWindow::_orientation_changed() {
	QRect screenGeometry = QApplication::desktop()->screenGeometry();
	if (screenGeometry.width() > screenGeometry.height()) {
		_player_form->landscapeMode();
		_library_form->landscapeMode();
		_equalizer_dialog->landscapeMode();
	} else {
		_player_form->portraitMode();
		_library_form->portraitMode();
		_equalizer_dialog->portraitMode();
	}
}

void MainWindow::_fullscreen(bool f) {
	if (f) showFullScreen();
	else showNormal();
}
