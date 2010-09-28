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

#include "saveplaylistdialog.h"
#include "ui_saveplaylistdialog.h"

SavePlaylistDialog::SavePlaylistDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SavePlaylistDialog)
{
	ui->setupUi(this);
	selectedItem = "";
	connect(ui->listWidget, SIGNAL(activated(QModelIndex)), this, SLOT(_select_item(QModelIndex)));
}

SavePlaylistDialog::~SavePlaylistDialog()
{
	delete ui;
}

void SavePlaylistDialog::setPlaylistNames(QList<QString> names) {
	ui->listWidget->addItems(names);
	ui->lineEdit->setText("New playlist");
}

QString SavePlaylistDialog::selectedName() {
	if (selectedItem.isEmpty())
		return ui->lineEdit->text();
	else return ui->listWidget->selectedItems().at(0)->text();
}

void SavePlaylistDialog::_select_item(QModelIndex id) {
	selectedItem = id.data().toString();
	done(QDialog::Accepted);
}

