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

#ifndef SAVEPLAYLISTDIALOG_H
#define SAVEPLAYLISTDIALOG_H

#include <QDialog>
#include <QList>
#include <QModelIndex>

namespace Ui {
	class SavePlaylistDialog;
}

class SavePlaylistDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SavePlaylistDialog(QString suggestName, QWidget *parent = 0);
	~SavePlaylistDialog();
	void setPlaylistNames(QList<QString>);
	QString selectedName();

public slots:
	void updateTranslations();

private slots:
	void _select_item(QModelIndex);
private:
	Ui::SavePlaylistDialog *ui;
	QString selectedItem;
};

#endif // SAVEPLAYLISTDIALOG_H
