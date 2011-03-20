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

#include "managelibraryform.h"
#include "ui_managelibraryform.h"
#include "library.h"
#include <QFileDialog>
#include <QDebug>

#define DEFAULT_PATH_PREFIX "/home/user/MyDocs/"

using namespace SomePlayer::DataObjects;
using namespace SomePlayer::Storage;

ManageLibraryForm::ManageLibraryForm(Library *library, QWidget *parent) :
		QWidget(parent),
		ui(new Ui::ManageLibraryForm),
		_library (library)
{
	ui->setupUi(this);
	setAttribute(Qt::WA_Maemo5StackedWindow);
	setWindowFlags(Qt::Window | windowFlags());
	_model = new QStandardItemModel(0, 2, this);
	ui->dirView->setModel(_model);
	_busy_widget = new BusyWidget(this);
	ui->mainLayout->addWidget(_busy_widget);
	_busy_widget->hide();
	connect(ui->addButton, SIGNAL(clicked()), this, SLOT(_add()));
	connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(_delete_selected()));
	connect(ui->updateButton, SIGNAL(clicked()), this, SLOT(_update_selected()));
	connect(ui->updateAllButton, SIGNAL(clicked()), this, SLOT(_update_all()));
	connect(ui->updatePlsButton, SIGNAL(clicked()), _library, SLOT(updatePlaylists()));
	connect(_library, SIGNAL(allCount(int)), _busy_widget, SLOT(setMax(int)));
	connect(_library, SIGNAL(tick()), _busy_widget, SLOT(tick()));
	connect(_library, SIGNAL(done()), _busy_widget, SLOT(hide()));
	connect(_library, SIGNAL(started()), _busy_widget, SLOT(show()));
	connect(_library, SIGNAL(done()), this, SLOT(refresh()));
	connect(ui->dirView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
		this, SLOT(_process_selection(QItemSelection,QItemSelection)));
	Config config;
	_icons_theme = config.getValue("ui/iconstheme").toString();
}

ManageLibraryForm::~ManageLibraryForm()
{
	delete ui;
}

void ManageLibraryForm::_add() {
	QString directory = QFileDialog::getExistingDirectory (this, tr("Select directory"), DEFAULT_PATH_PREFIX, QFileDialog::ShowDirsOnly );
	if (!directory.isEmpty()) {
		_library->addDirectory(directory);
	}
}

void ManageLibraryForm::refresh() {
	QList<QString> directories = _library->getDirectories();
	qSort(directories);
	int artists_count = _library->getArtistsCount();
	int albums_count = _library->getAlbumsCount();
	int tracks_count = _library->getTracksCount();

	ui->artistsLabel->setText(QString("%1 artists").arg(artists_count));
	ui->albumsLabel->setText(QString("%1 albums").arg(albums_count));
	ui->tracksView->setText(QString("%1 tracks").arg(tracks_count));

	int dcount = directories.count();
	_model->setRowCount(dcount);
	for (int i = 0; i < dcount; i++) {
		_model->setItem(i, 0, new QStandardItem(QIcon(":/icons/"+_icons_theme+"/deselect_all.png"), ""));
		QString dir = directories.at(i);
		dir.replace(DEFAULT_PATH_PREFIX, "");
		_model->setItem(i, 1, new QStandardItem(dir));
	}
	ui->dirView->setColumnWidth(0, 70);
}

void ManageLibraryForm::updateIcons() {
	Config config;
	_icons_theme = config.getValue("ui/iconstheme").toString();
	ui->addButton->setIcon(QIcon(":/icons/"+_icons_theme+"/add.png"));
	ui->deleteButton->setIcon(QIcon(":/icons/"+_icons_theme+"/delete.png"));
	ui->updateButton->setIcon(QIcon(":/icons/"+_icons_theme+"/update.png"));
	ui->updateAllButton->setIcon(QIcon(":/icons/"+_icons_theme+"/update-all.png"));
	ui->updatePlsButton->setIcon(QIcon(":/icons/"+_icons_theme+"/update-pls.png"));
	refresh();
}

void ManageLibraryForm::_process_selection(QItemSelection selected, QItemSelection deselected) {
	foreach (QModelIndex id, selected.indexes()) {
		if (id.column() == 0) {
			_model->item(id.row())->setIcon(QIcon(":/icons/"+_icons_theme+"/select_all.png"));
		}
		ui->dirView->selectionModel()->select(id, QItemSelectionModel::Select);
	}
	foreach (QModelIndex id, deselected.indexes()) {
		if (id.column() == 0) {
			_model->item(id.row())->setIcon(QIcon(":/icons/"+_icons_theme+"/deselect_all.png"));
		}
		ui->dirView->selectionModel()->select(id, QItemSelectionModel::Deselect);
	}
}

void ManageLibraryForm::_delete_selected() {
	QList<QString> directories;
	QModelIndexList idx = ui->dirView->selectionModel()->selectedIndexes();
	if (idx.count() == 0) {
		return;
	}
	CONFIRM_ACTION(this, tr("Delete selected directories?"))
	foreach (QModelIndex id, idx) {
		if (id.column() == 1) {
			QString path = id.data().toString();
			if (!path.startsWith("/")) {
				path = DEFAULT_PATH_PREFIX+path;
			}
			directories.append(path);
		}
	}
	if (!directories.isEmpty()) {
		_library->deleteDirectories(directories);
	}
	ui->dirView->selectionModel()->clearSelection();
	refresh();
	emit refreshLibrary();
}

void ManageLibraryForm::_update_selected() {
	CONFIRM_ACTION(this, tr("Update selected directories? It may take several minutes"))
	QList<QString> directories;
	QModelIndexList idx = ui->dirView->selectionModel()->selectedIndexes();
	foreach (QModelIndex id, idx) {
		if (id.column() == 1) {
			QString path = id.data().toString();
			if (!path.startsWith("/")) {
				path = DEFAULT_PATH_PREFIX+path;
			}
			directories.append(path);
		}
	}
	if (!directories.isEmpty()) {
		_library->updateDirectories(directories);
	}
	refresh();
	emit refreshLibrary();
}

void ManageLibraryForm::_update_all() {
	CONFIRM_ACTION(this, tr("Update the entire library? It may take a long time"))
	_library->updateAll();
	refresh();
	emit refreshLibrary();
}

void ManageLibraryForm::updateTranslations() {
	ui->retranslateUi(this);
}
