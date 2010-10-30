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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "playerform.h"
#include "libraryform.h"
#include "busywidget.h"
#include "equalizerdialog.h"
#include "managelibraryform.h"
#include "directoryview.h"
#include "library.h"
#include <QTimer>

namespace Ui {
	class MainWindow;
}

using SomePlayer::DataObjects::Library;
using SomePlayer::DataObjects::Track;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindow *ui;

signals:
	void landscapeMode();
	void portraitMode();

public slots:
	void about();
	void player();
	void library();
	void settings();
private slots:
	void _manage_library();
	void _save_playlist();
	void _clear_current_playlist();
	void _set_timer();
	void _timeout();
	void _equalizer();
	void _equalizer_value_changed(int, int);
	void _orientation_changed();
	void _fullscreen(bool);
	void _add_tracks(QList<Track>);
private:
	PlayerForm *_player_form;
	LibraryForm *_library_form;
	BusyWidget *_busy_widget;
	DirectoryView *_directory_form;
	Library *_library;
	QTimer *_timer;
	EqualizerDialog *_equalizer_dialog;
	ManageLibraryForm *_manage_library_form;
};

#endif // MAINWINDOW_H
