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

#include "directoryview.h"
#include "ui_directoryview.h"
#include <QDir>
#include <QFileInfo>
#include "someplayer.h"
#include <QDebug>
#include <QRegExp>
#include "config.h"

using namespace SomePlayer::Storage;
using namespace SomePlayer::DataObjects;

DirectoryView::DirectoryView(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::DirectoryView)
{
	ui->setupUi(this);
	Config config;
	_icons_theme = config.getValue("ui/iconstheme").toString();
	REGISTERED_FILE_EXTENSIONS << "mp3" << "flac" << "wma" << "aac" << "ogg" << "asf" << "ape" << "wav" << "m4a";
	setWindowFlags(windowFlags() | Qt::Window);
	setAttribute(Qt::WA_Maemo5StackedWindow);
	_model = new QStandardItemModel(0, 2, this);
	ui->dirView->setModel(_model);
	ui->dirView->setColumnHidden(2, true);
	_current_dir = QDir::homePath();
	_tagresolver = new TagResolver(this);
	_mediascanner = new MediaScanner(this);

	ui->addButton->setEnabled(false);
	ui->addButton->setIcon(QIcon());
	ui->useButton->setEnabled(false);
	ui->useButton->setIcon(QIcon());

	ui->progressBar->hide();

	_home();
	connect(ui->backButton, SIGNAL(clicked()), this, SLOT(_back()));
	connect(ui->dirView, SIGNAL(clicked(QModelIndex)), this, SLOT(_process_click(QModelIndex)));
	connect(ui->dirView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(_process_dblclick(QModelIndex)));
	connect(ui->dirView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this,
		SLOT(_process_selection(QItemSelection,QItemSelection)));
	connect(ui->homeButton, SIGNAL(clicked()), this, SLOT(_home()));
	connect(ui->selectToggleButton, SIGNAL(clicked()), this, SLOT(_toggle_selection()));
	connect(ui->addButton, SIGNAL(clicked()), this, SLOT(_add()));
	connect(ui->useButton, SIGNAL(clicked()), this, SLOT(_use()));
	connect(_tagresolver, SIGNAL(decoded(Track)), this, SLOT(_add_track(Track)));
	connect(_tagresolver, SIGNAL(done()), this, SLOT(_done()));
	connect(ui->playerButton, SIGNAL(clicked()), this, SLOT(hide()));
	_top_gradient = ui->topWidget->styleSheet();
	_bottom_gradient = ui->bottomWidget->styleSheet();
}

DirectoryView::~DirectoryView()
{
	delete ui;
}

void DirectoryView::readDir(QString path, QString pathToScroll) {
	QDir dir(path);
	_current_dir = dir.absoluteFilePath(path);
	_directories.clear();
	_files.clear();
	QList<QString> dirnames;
	QList<QString> filenames;
	QFileInfoList items = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden);
	foreach (QFileInfo info, items) {
		if (info.isDir()) {
			if (info.fileName().startsWith(".")) {
				if (info.fileName() == ".sounds") {
					_directories << info.absoluteFilePath();
					dirnames << tr("Audioclips");
				}
			} else {
				_directories << info.absoluteFilePath();
				dirnames << info.fileName();
			}
		} else {
			if (REGISTERED_FILE_EXTENSIONS.contains(info.suffix().toLower())) {
				_files << info.absoluteFilePath();
				filenames << info.fileName();
			}
		}
	}
	_model->setRowCount(dirnames.count() + filenames.count());
	int i = 0;
	foreach (QString str, dirnames) {
		_model->setItem(i, 0, new QStandardItem(QIcon(":/icons/"+_icons_theme+"/deselect_all.png"), ""));
		_model->setItem(i, 1, new QStandardItem(QIcon(":/icons/"+_icons_theme+"/folder.png"), str));
		i++;
	}
	foreach (QString str, filenames) {
		_model->setItem(i, 0, new QStandardItem(QIcon(":/icons/"+_icons_theme+"/deselect_all.png"), ""));
		_model->setItem(i, 1, new QStandardItem(str));
		i++;
	}
	ui->dirView->setColumnWidth(0, 70);
	ui->addButton->setEnabled(false);
	ui->addButton->setIcon(QIcon());
	ui->useButton->setEnabled(false);
	ui->useButton->setIcon(QIcon());
	if (pathToScroll.isEmpty()) {
		ui->dirView->scrollToTop();
		return;
	}
	int row = _directories.indexOf(pathToScroll);
	if (row >= 0 && row < _model->rowCount()) {
		QModelIndex id = _model->index(row, 1);
		ui->dirView->scrollTo(id, QAbstractItemView::PositionAtCenter);
	}
}

void DirectoryView::_back() {
	QDir current(_current_dir);
	current.cdUp();
	QString dir_to_center = "";
	if (_nav_history.size() > 0) {
		dir_to_center = _nav_history.pop();
	}
	readDir(current.path(), dir_to_center);
}

void DirectoryView::_process_click(QModelIndex index) {
	if (index.column() == 0) {
	} else {
		if (index.row() < _directories.count()) {
			_current_dir = _directories.at(index.row());
			readDir(_current_dir);
			_nav_history.push(_current_dir);
			return;
		}
	}
}

void DirectoryView::_process_dblclick(QModelIndex index) {
	if (index.column() == 0) {
	} else {
		if (index.row() >= _directories.count()) {
			QString filename = _files.at(index.row() - _directories.count());
			Track track = _tagresolver->decodeOne(filename);
			emit addAndPlay(track);
			return;
		}
	}
}

void DirectoryView::_process_selection(QItemSelection selected, QItemSelection deselected) {
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
	if (ui->dirView->selectionModel()->selectedRows().count() > 0) {
		ui->addButton->setEnabled(true);
		ui->addButton->setIcon(QIcon(":/icons/"+_icons_theme+"/add.png"));
		ui->useButton->setEnabled(true);
		ui->useButton->setIcon(QIcon(":/icons/"+_icons_theme+"/use.png"));
	} else {
		ui->addButton->setEnabled(false);
		ui->addButton->setIcon(QIcon());
		ui->useButton->setEnabled(false);
		ui->useButton->setIcon(QIcon());
	}
}

void DirectoryView::_home() {
	_current_dir = QDir::homePath();
	homeScreen();
	_nav_history.clear();
}

void DirectoryView::_toggle_selection() {
	if (ui->dirView->selectionModel()->selectedRows().count() == _model->rowCount()) {
		ui->dirView->clearSelection();
		ui->selectToggleButton->setIcon(QIcon(":/icons/"+_icons_theme+"/select_all.png"));
	} else {
		ui->dirView->selectAll();
		ui->selectToggleButton->setIcon(QIcon(":/icons/"+_icons_theme+"/deselect_all.png"));
	}
}

void DirectoryView::_add() {
	QModelIndexList selected = ui->dirView->selectionModel()->selectedRows(1);
	QStringList files;
	QStringList directories;
	foreach (QModelIndex index, selected) {
		if (index.row() >= _directories.count()) {
			files << _files.at(index.row() - _directories.count());
		} else {
			directories << _directories.at(index.row());
		}
	}
	foreach (QString dir, directories) {
		files.append(_mediascanner->singleScan(dir));
	}
	_tracks.clear();
	ui->progressBar->setMinimum(0);
	ui->progressBar->setMaximum(files.count());
	ui->progressBar->show();
	_tagresolver->decode(files);
	ui->dirView->clearSelection();
}

void DirectoryView::_add_track(Track track) {
	_tracks.append(track);
	ui->progressBar->setValue(ui->progressBar->value()+1);
}

void DirectoryView::_done() {
	emit addTracks(_tracks);
	ui->progressBar->hide();
	_tracks.clear();
}

void DirectoryView::updateIcons() {
	Config config;
	_icons_theme = config.getValue("ui/iconstheme").toString();
	if (!ui->addButton->icon().isNull())
		ui->addButton->setIcon(QIcon(":/icons/"+_icons_theme+"/add.png"));
	if (!ui->useButton->icon().isNull())
		ui->useButton->setIcon(QIcon(":/icons/"+_icons_theme+"/add.png"));
	ui->homeButton->setIcon(QIcon(":/icons/"+_icons_theme+"/home.png"));
	ui->backButton->setIcon(QIcon(":/icons/"+_icons_theme+"/back.png"));
	if (ui->dirView->selectionModel()->selectedRows().count() == _model->rowCount()) {
		ui->selectToggleButton->setIcon(QIcon(":/icons/"+_icons_theme+"/deselect_all.png"));
	} else {
		ui->selectToggleButton->setIcon(QIcon(":/icons/"+_icons_theme+"/select_all.png"));
	}
	ui->playerButton->setIcon(QIcon(":/icons/"+_icons_theme+"/player.png"));
	_home();
}

void DirectoryView::checkGradient() {
	Config config;
	if (config.getValue("ui/gradient").toString() == "yes") {
		ui->bottomWidget->setStyleSheet(_bottom_gradient);
		ui->topWidget->setStyleSheet(_top_gradient);
	} else {
		ui->topWidget->setStyleSheet("");
		ui->bottomWidget->setStyleSheet("");
	}
}

void DirectoryView::lanscapeMode() {
	ui->topWidget->hide();
	ui->bottomWidget->hide();
	ui->lverticalLayout->removeItem(ui->lverticalSpacer_0);
	ui->lverticalLayout->removeItem(ui->lverticalSpacer_1);
	ui->lverticalLayout->addWidget(ui->backButton);
	ui->lverticalLayout->addItem(ui->lverticalSpacer_0);
	ui->lverticalLayout->addWidget(ui->homeButton);
	ui->lverticalLayout->addItem(ui->lverticalSpacer_1);
	ui->lverticalLayout->addWidget(ui->playerButton);
	ui->rverticalLayout->removeItem(ui->rverticalSpacer_0);
	ui->rverticalLayout->removeItem(ui->rverticalSpacer_1);
	ui->rverticalLayout->addWidget(ui->addButton);
	ui->rverticalLayout->addItem(ui->rverticalSpacer_0);
	ui->rverticalLayout->addWidget(ui->useButton);
	ui->rverticalLayout->addItem(ui->rverticalSpacer_1);
	ui->rverticalLayout->addWidget(ui->selectToggleButton);
	ui->lverticalWidget->show();
	ui->rverticalWidget->show();
}

void DirectoryView::portraitMode() {
	ui->lverticalWidget->hide();
	ui->rverticalWidget->hide();
	ui->lverticalLayout->removeItem(ui->lverticalSpacer_0);
	ui->lverticalLayout->removeItem(ui->lverticalSpacer_1);
	ui->rverticalLayout->removeItem(ui->rverticalSpacer_0);
	ui->topWidget->layout()->removeItem(ui->thorizontalSpacer_0);
	ui->topWidget->layout()->removeItem(ui->thorizontalSpacer_1);
	ui->topWidget->layout()->addWidget(ui->backButton);
	ui->topWidget->layout()->addItem(ui->thorizontalSpacer_0);
	ui->topWidget->layout()->addWidget(ui->homeButton);
	ui->topWidget->layout()->addItem(ui->thorizontalSpacer_1);
	ui->topWidget->layout()->addWidget(ui->addButton);
	ui->bottomWidget->layout()->removeItem(ui->bhorizontalSpacer_0);
	ui->bottomWidget->layout()->removeItem(ui->bhorizontalSpacer_1);
	ui->bottomWidget->layout()->addWidget(ui->playerButton);
	ui->bottomWidget->layout()->addItem(ui->bhorizontalSpacer_0);
	ui->bottomWidget->layout()->addWidget(ui->useButton);
	ui->bottomWidget->layout()->addItem(ui->bhorizontalSpacer_1);
	ui->bottomWidget->layout()->addWidget(ui->selectToggleButton);
	ui->topWidget->show();
	ui->bottomWidget->show();
}

void DirectoryView::updateTranslations() {
	ui->retranslateUi(this);
	homeScreen();
}

void DirectoryView::homeScreen() {
	_directories.clear();
	_files.clear();
	_model->clear();
	_model->setRowCount(2);
	_model->setColumnCount(2);
	_model->setItem(0, 0, new QStandardItem(QIcon(":/icons/"+_icons_theme+"/deselect_all.png"), ""));
	_model->setItem(0, 1, new QStandardItem(tr("Internal memory")));
	_model->setItem(1, 0, new QStandardItem(QIcon(":/icons/"+_icons_theme+"/deselect_all.png"), ""));
	_model->setItem(1, 1, new QStandardItem(tr("Memory card")));
	_directories << QDir::homePath()+"/MyDocs/";
	_directories << "/media/mmc1/";
	ui->dirView->setColumnWidth(0, 70);
	ui->addButton->setEnabled(false);
	ui->addButton->setIcon(QIcon());
	ui->useButton->setEnabled(false);
	ui->useButton->setIcon(QIcon());
}

void DirectoryView::_use() {
	emit clearCurrent();
	_add();
}
