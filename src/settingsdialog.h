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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "config.h"

namespace Ui {
	class SettingsDialog;
}

class SettingsDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SettingsDialog(QWidget *parent = 0);
	~SettingsDialog();

private:
	Ui::SettingsDialog *ui;

private slots:
	void _set_album_sorting_alphabet(bool);
	void _set_album_sorting_date(bool);
	void _set_track_lenght_show_yes(bool);
	void _set_track_lenght_show_no(bool);
	void _set_orientation_auto(bool);
	void _set_orientation_portrait(bool);
	void _set_orientation_landscape(bool);
	void _set_icons_white(bool);
	void _set_icons_black(bool);
	void _set_gradient_yes(bool);
	void _set_gradient_no(bool);
};

#endif // SETTINGSDIALOG_H
