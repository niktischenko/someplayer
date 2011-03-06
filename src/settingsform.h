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


#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QWidget>
#include "config.h"

namespace Ui {
	class SettingsForm;
}

class SettingsForm : public QWidget {
	Q_OBJECT

public:
	explicit SettingsForm(QWidget *parent = 0);
	~SettingsForm();

public slots:
	void updateTranslations();
	void landscapeMode();
	void portraitMode();

signals:
	void iconsChanged();
	void orientationChanged();
	void gradientChanged();
	void libraryOptionsChanged();
	void translationChanged();
	void trackColorChanged();
	void hwZoomPolicyChanged();
	void fmtxSettingsChanged();

private:
	Ui::SettingsForm *ui;
	QStringList _color_map;
	QStringList _lang_map;

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
	void _set_pause_hp_yes(bool);
	void _set_pause_hp_no(bool);
	void _toggle_hw_settings(bool);
	void _set_hw_volume_control(bool);
	void _set_hw_track_control(bool);
	void _toggle_view_ui();
	void _toggle_view_lib();
	void _toggle_view_hw();
	void _open_fmtx_settings();
	void _toggle_fmtx_settings(bool);
	void _set_color(int);
	void _set_language(int);
};

#endif // SETTINGSFORM_H
