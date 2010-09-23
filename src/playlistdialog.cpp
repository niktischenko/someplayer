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


#include "playlistdialog.h"
#include "ui_playlistdialog.h"
#include <QStandardItemModel>

PlaylistDialog::PlaylistDialog(QList<QString> playlistNames, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlaylistDialog)
{
    ui->setupUi(this);
    QStandardItemModel *model = new QStandardItemModel(this);
    foreach (QString str, playlistNames) {
	    model->appendRow(new QStandardItem(str));
    }
    ui->listView->setModel(model);
    setWindowTitle("Playlists");
}

PlaylistDialog::~PlaylistDialog()
{
    delete ui;
}

QList<QString> PlaylistDialog::selected() {
	QList<QString> _selected;
	QList<QModelIndex> idx = ui->listView->selectionModel()->selectedIndexes();
	foreach (QModelIndex id, idx) {
		_selected.append(id.data().toString());
	}
	return _selected;
}
