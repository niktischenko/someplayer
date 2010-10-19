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

using namespace SomePlayer::DataObjects;

ManageLibraryForm::ManageLibraryForm(Library *library, QWidget *parent) :
		QWidget(parent),
		ui(new Ui::ManageLibraryForm),
		_library (library)
{
	ui->setupUi(this);
	connect(ui->addButton, SIGNAL(clicked()), this, SLOT(add()));
}

ManageLibraryForm::~ManageLibraryForm()
{
	delete ui;
}

void ManageLibraryForm::add() {
	QString directory = QFileDialog::getExistingDirectory (this, "Select directory", "/home/user/MyDocs", QFileDialog::ShowDirsOnly );
	if (!directory.isEmpty()) {
		_library->addDirectory(directory);
	}
}
