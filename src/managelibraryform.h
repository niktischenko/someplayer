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

#ifndef MANAGELIBRARYFORM_H
#define MANAGELIBRARYFORM_H

#include <QWidget>
#include "someplayer.h"

namespace Ui {
	class ManageLibraryForm;
}

using SomePlayer::DataObjects::Library;

class ManageLibraryForm : public QWidget
{
	Q_OBJECT

public:
	explicit ManageLibraryForm(Library *library, QWidget *parent = 0);
	~ManageLibraryForm();

private slots:
	void add();

private:
	Ui::ManageLibraryForm *ui;
	Library *_library;
};

#endif // MANAGELIBRARYFORM_H
